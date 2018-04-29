#include "tcp_server.h"
#include "string.h"

#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>

#include <functional>
#include <sstream>
#include <iostream>


namespace srv {


	TcpServer::TcpServer(int port, std::size_t lc) :
		port_(port), lc_(lc) {
        stat_.store(0);
	}

	TcpServer::~TcpServer() {
        stop();
	}

	bool TcpServer::Init() {
		if (port_ <= 0) return false;
		if (lc_ <= 0 || lc_ > 4) return false;

		//worker_pool_.reset(new thp::ThreadPool(1000, lc_));
		//if (!worker_pool_) return false;
        //if (!worker_pool_->Init()) return false;

		worker_pool_.reset(new evthp::EvThreadPool(3 * lc_));
		if (!worker_pool_) return false;
        if (!worker_pool_->Init()) return false;

        for (std::size_t i = 0; i < lc_; ++i) {
            ListenerPtr ln(new Listener());
            if (!ln) {
                return false;
            }

            ln->stat_.store(3);  //stopped
            ln->th_.reset();

            listeners_.push_back(ln);
        }

        stat_.store(1); // created

		return true;
	}

	bool TcpServer::Start() {
        if (!worker_pool_->Start()) return false;

        stat_.store(2); // started and running

        while (stat_.load() != 3) {
            for (std::size_t i = 0; i < listeners_.size(); ++i) {
                ListenerPtr& ln = listeners_[i];
                if (ln->stat_.load() != 1) {
                    ln->th_.reset(new std::thread(std::bind(
                                    &TcpServer::listen_main, this, ln)));
                }
            }

            std::chrono::milliseconds tv(1000) ;
            std::this_thread::sleep_for(tv);
        }

        return true;
	}

	void TcpServer::Stop() {
        stop();
    }

	void TcpServer::stop() {
        stat_.store(3); // stopping

        for (std::size_t i = 0; i < listeners_.size(); ++i) {
            listeners_[i]->stat_.store(2);
            listeners_[i]->th_->join();
        }

        worker_pool_->Stop();

        stat_.store(1);

        return;
	}

    void TcpServer::Handle(int fd, std::thread::id& dispatcher) {
        int efd = epoll_create(64);
        if (-1 == efd) {
            return;
        }

        struct epoll_event ev;
        // EPOLLERR and EPOLLHUP will be added automatically
        ev.events = EPOLLIN | EPOLLRDHUP;
        ev.data.fd = fd;
        int rv = epoll_ctl(efd, EPOLL_CTL_ADD, fd, &ev);
        if (-1 == rv) {
            goto errexit;
        }

        struct epoll_event revents[1];
        memset(revents, 0, sizeof(revents));
        rv = epoll_wait(efd, revents, 1, 100);
        if (rv <= 0) {  // error or timeout
            goto errexit;
        } else {
            if (EPOLLERR & revents[0].events ||
                    EPOLLHUP & revents[0].events) {
                goto errexit;
            }
            
            if (EPOLLIN & revents[0].events || 
                    EPOLLRDHUP & revents[0].events) {
                std::ostringstream oss;
                std::thread::id tid = std::this_thread::get_id();
                oss << "[" << dispatcher << ":" << tid << "]: ";

                char buff[1024];
                while (true) {
                    memset(buff, 0, sizeof(buff));
                    ssize_t rc = recv(fd, buff, sizeof(buff)-1, 0);
                    if (rc < 0) {
                        oss << "server read error";
                        break;
                    } else if (rc == 0) {
                        break;
                    } else {
                        std::string tmpstr(buff);
                        oss << tmpstr;
                        if (rc < (ssize_t)(sizeof(buff) - 1)) {
                            break;
                        }
                    }
                }

                std::string res = oss.str();
                send(fd, res.data(), res.size(), 0);
            }
        }

errexit:
        close(fd);
        if (efd > 0) close(efd);
        return;
    }

    void TcpServer::dispatch(int fd) {
        thp::Job j = std::bind(&TcpServer::Handle,
                this, fd, std::this_thread::get_id()); 

        int retries = 10;
        bool ok = false;
        while (!ok && retries-- > 0) {
            //ok = worker_pool_->AddJob(j);
            ok = worker_pool_->Dispatch(j);
            if (!ok) {
                std::chrono::milliseconds tv(20) ;
                std::this_thread::sleep_for(tv);
            }
        }

        if (!ok) {
            close(fd);
        }
    }

	void TcpServer::listen_main(ListenerPtr& t) {
        t->stat_.store(1);

		int so = socket(AF_INET,
				SOCK_STREAM|SOCK_CLOEXEC|SOCK_NONBLOCK, 0);
		if (-1 == so) {
            t->stat_.store(3);  // stopped
            return;
		}

		int reuseport = 1;
		if (-1 == setsockopt(so, SOL_SOCKET, SO_REUSEPORT,
					(void*)&reuseport, sizeof(reuseport))) {
            close(so);
            t->stat_.store(3);  // stopped
            return;
		}

		struct sockaddr_in addr;
		memset(&addr, 0, sizeof(struct sockaddr_in));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port_);
		// INADDR_ANY
		if (0 == inet_aton("0.0.0.0", &addr.sin_addr)) {
            close(so);
            t->stat_.store(3);  // stopped
            return;
		}

		if (-1 == bind(so, (struct sockaddr*)&addr,
                    sizeof(struct sockaddr_in))) {
            close(so);
            t->stat_.store(3);  // stopped
            return;
		}

		if (-1 == listen(so, 512)) {
            close(so);
            t->stat_.store(3);  // stopped
            return;
		}

        int efd = epoll_create(64);
        if (-1 == efd) {
            close(so);
            t->stat_.store(3);  // stopped
            return;
        }

        struct epoll_event ev;
        // EPOLLERR and EPOLLHUP will be added automatically
        ev.events = EPOLLIN;
        ev.data.fd = so;
        int rv = epoll_ctl(efd, EPOLL_CTL_ADD, so, &ev);
        if (-1 == rv) {
            close(so);
            close(efd);
            t->stat_.store(3);  // stopped
            return;
        }

        struct epoll_event revents[1];
        while (true) {
            memset(revents, 0, sizeof(revents));
            int rv = epoll_wait(efd, revents, 1, 1000);
            if (-1 == rv) { // handle error
                if (errno == EINTR) {
                    continue;
                } else {
                    break;
                }
            }

            if (rv > 0) {   // handle accept 
                if (EPOLLIN & revents[0].events) {
                    struct sockaddr_in rv_addr;
                    socklen_t rv_addrlen;
                    int fd = accept(revents[0].data.fd,
                            (struct sockaddr*)&rv_addr, &rv_addrlen);
                    if (fd > 0) {
                        dispatch(fd);
                    }
                } 
            }

            // handle timeout
            if (t->stat_.load() == 2) {    // stopping
                break;
            } 
        }

        close(so);
        close(efd);
        t->stat_.store(3);  // stopped

        return;

	}


}
