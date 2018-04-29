#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "event2/event.h"

#include "evthread_pool.h"



namespace evthp {

    JobQueue::JobQueue(std::size_t mx):
        max_(mx) {
    }

    JobQueue::~JobQueue() {
    }

    bool JobQueue::Push(const Job& j) {
        std::chrono::milliseconds tv(100);
        if (!tmtx_.try_lock_for(tv)) {
            return false;
        }

        jobs_.push_back(j);

        tmtx_.unlock();

        return true;
    }

    Job JobQueue::Pop() {
        Job j;

        std::chrono::milliseconds tv(100);
        if (!tmtx_.try_lock_for(tv)) {
            return j;
        }

        if (!jobs_.empty()) {
            j = jobs_.front();
            jobs_.pop_front();
        }

        tmtx_.unlock();

        return j;
    }


    










    EvThread::EvThread(std::size_t mx) : max_(mx), eb_(NULL) {
    }

    EvThread::~EvThread() {
        stop();
        if (eb_ != NULL) {
            event_base_free(eb_);
        }
    }

    bool EvThread::AddJob(const Job& j) {
        if (queue_->Push(j)) {
            unsigned char c = 1;        // 1 - new job
            send(ch_[0], &c, 1, 0);

            return true;
        }

        return false;
    }

    bool EvThread::Ping() {
        unsigned char c = 2; // 2 - ping msg

        int stimes = 10;
        while (stimes-- > 0) {
            if (1 == send(ch_[0], &c, 1, 0)){
                break;
            }

            std::chrono::milliseconds tv(1);
            std::this_thread::sleep_for(tv);
        }

        if (stimes == 0) {
            return false;
        }

        int rtimes = 100;
        while (rtimes-- > 0) { 
            unsigned char r = 0;
            if (1 == recv(ch_[0], &r, 1, 0) && r == 2) {
                return true;
            }

            // by this we make sure that a job does not take
            // more than 100 * 10 ms time, otherwise, we will
            // reset the thread
            std::chrono::milliseconds tv(10);
            std::this_thread::sleep_for(tv);
        }

        return false;

    }

    bool EvThread::Start() {
        if (0 > socketpair(AF_UNIX,
                    SOCK_STREAM|SOCK_NONBLOCK|SOCK_CLOEXEC, 0, ch_)) {
            return false;
        }

        queue_.reset(new JobQueue(max_));
        if (!queue_) {
            return false;
        }

        worker_.reset(new std::thread(std::bind(&EvThread::threadMain, this)));
        if (!worker_) {
            return false;
        }

        return true;
    }

    void EvThread::Stop() {
        stop();
    }

    void EvThread::stop() {
        if (ch_[0] > 0 && ch_[1] > 0) {
            //std::cerr << "[Hypnos]: invoke EvThread::stop() in " <<
            //    std::this_thread::get_id() << std::endl;

            char c = 0; // 0 - stop, 1 - new job, 2 - ping

            //send 5 times to make sure that the thread received the directive
            int stimes = 5;
            while (stimes-- > 0) {
                send(ch_[0], &c, 1, 0);

                std::chrono::milliseconds tv(1);
                std::this_thread::sleep_for(tv);
            }

            close(ch_[0]);
            close(ch_[1]);

            ch_[0] = 0;
            ch_[1] = 0;

            // may block, we need a method forcing the thread stop
            // or a component like the Context in golang
            worker_->join();
        }
    }

    void EvThread::threadMain() {
        eb_ = event_base_new();
        if (eb_ == NULL) return;

        struct event* evch = event_new(eb_, ch_[1],
                EV_READ|EV_PERSIST,
                EvHandle,
                this);
        if (evch == NULL) return;

        if (0 > event_add(evch, NULL)) {
            return;
        }

        //if (0 > event_base_loop(eb_, EVLOOP_NO_EXIT_ON_EMPTY)) {
        //    return;
        //}

        if (0 > event_base_dispatch(eb_)) {
            return;
        }

        return;
    }

    void EvThread::EvHandle(int fd, short what, void *arg) {
        unsigned char r = 0;

        EvThread* etp = reinterpret_cast<EvThread*>(arg);

        int* ch = etp->GetCh();
        if (1 != recv(ch[1], &r, 1, 0)) {
            // we may encounter some errors
            event_base_loopexit(etp->GetEvBase(), NULL);
            return;
        }

        switch (r) {
            case 0:
                EvHandleStop(etp);
                break;
            case 1:
                EvHandleJob(etp);
                break;
            case 2:
                EvHandlePing(etp);
                break;
            default:
                break;
        }

        return;
    }

    void EvThread::EvHandleStop(EvThread* etp) {
        event_base_loopexit(etp->GetEvBase(), NULL);
    }

    void EvThread::EvHandlePing(EvThread* etp) {
        unsigned char r = 2;

        int* ch = etp->GetCh();
        int stimes = 5;
        while (stimes-- > 0) {
            if (1 == send(ch[1], &r, 1, 0)) {
                break;
            }

            std::chrono::milliseconds tv(1);
            std::this_thread::sleep_for(tv);
        }
    }

    void EvThread::EvHandleJob(EvThread* etp) {
        Job j = etp->GetQueue()->Pop();
        if (j) {
            j();
        }
    }










    EvThreadPool::EvThreadPool(std::size_t tc):tc_(tc), seq_(0) {
        stat_.store(0);
    }

    EvThreadPool::~EvThreadPool() {
        stop();
    } 

    bool EvThreadPool::Init() {
        if (tc_ <= 0) return false;

        for (std::size_t i = 0; i < tc_; ++i) {
            EvThreadPtr ethp(new EvThread(1000));
            if (ethp) {
                workers_.push_back(std::move(ethp));
            } else {
                workers_.clear();
                return false;
            }
        }

        stat_.store(1);

        return true;
    }

    bool EvThreadPool::Start() {
        int retries = 3;

        while (retries-- > 0) {
            std::thread *thp = new std::thread(std::bind(&EvThreadPool::start, this));

            int t = 100;
            while (t-- > 0) {
                if (stat_.load() == 2) {
                    return true;
                }
                std::chrono::milliseconds tv(10);
                std::this_thread::sleep_for(tv);
            }

            delete thp;
        }

        return false;
    }

    void EvThreadPool::start() {
        for (auto it = workers_.begin();
                it != workers_.end(); ++it) {
            if (!(*it)->Start()) {
                for (auto tit = workers_.begin();
                        tit != it; ++tit) {
                    (*tit)->Stop();
                }
                return;
            }
        }

        stat_.store(2);

        while (stat_.load() != 3) {
            for (auto it = workers_.begin();
                    it != workers_.end(); ++ it) {
                if (!(*it)->Ping()) {
                    (*it)->Stop();
                    (*it)->Start();
                }
            }

            std::chrono::milliseconds tv(1000);
            std::this_thread::sleep_for(tv);
        }

        stop();
        
        return;
    }

    void EvThreadPool::Stop() {
        stat_.store(3);
    }

    void EvThreadPool::stop() {
        for (auto it = workers_.begin();
                it != workers_.end(); ++it) {
            (*it)->Stop();
        }

        stat_.store(1);
    }

    bool EvThreadPool::Dispatch(Job j) {
        if (stat_.load() != 2) {
            return false;
        }

        for (std::size_t i = 0; i < tc_; ++i) {
            std::size_t idx = (seq_ + i) % tc_;
            if (workers_[idx]->AddJob(j)) {
                seq_ = idx + 1;
                return true;
            }
        }

        return false;
    }


}
