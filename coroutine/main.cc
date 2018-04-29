#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <iostream>
#include <cstdlib>

#include "thread_pool.h"
#include "evthread_pool.h"
#include "tcp_server.h"

void run(int i) {
    std::cout << "I am job " << i << std::endl;
}

void thread_pool_test() {
    thp::ThreadPool tp(10, 4);
    if (!tp.Init()) {
        std::cerr << "thread pool initialization failed" << std::endl;
        return;
    }

    if (!tp.Start()) {
        std::cerr << "thread pool start failed" << std::endl;
        return;
    }

	int count = 0;
    for (int i = 0; i < 100; ++i) {
        thp::Job j = std::bind(run, i);
        int retries = 30;
        while (retries > 0) {
            if (tp.AddJob(j)) {
				count ++;
                break;
            } else {
                retries --;
                std::chrono::milliseconds tv(20) ;
                std::this_thread::sleep_for(tv);
            }
        }
    }

	std::cout << count << " tasks dispatched" << std::endl;

    std::chrono::seconds tv(10) ;
    std::this_thread::sleep_for(tv);
}

void evthread_pool_test() {
    evthp::EvThreadPool eth(10);
    if (!eth.Init()) {
        std::cerr << "evthread pool initialization failed" << std::endl;
        return;
    }

    std::cout << "evthread pool initialization ok" << std::endl;

    if (!eth.Start()) {
        std::cerr << "evthread pool start failed" << std::endl;
        return;
    }

    std::cout << "evthread pool start ok" << std::endl;

	int count = 0;
    for (int i = 0; i < 100; ++i) {
        evthp::Job j = std::bind(run, i);
        int retries = 30;
        while (retries > 0) {
            if (eth.Dispatch(j)) {
				count ++;
                break;
            } else {
                retries --;
                std::chrono::milliseconds tv(20) ;
                std::this_thread::sleep_for(tv);
            }
        }
    }

	std::cout << count << " tasks dispatched" << std::endl;

    std::chrono::seconds tv(10) ;
    std::this_thread::sleep_for(tv);

    eth.Stop();
}

void tcpserver_test() {
    srv::TcpServer tserv(60081, 4);

    if (!tserv.Init()) {
        std::cerr << "Initialize the tcp server failed" << std::endl;
        return;
    }

    if (!tserv.Start()) {
        std::cerr << "Start the tcp server failed" << std::endl;
        return;
    }

    std::cout << "The tcp server stopped" << std::endl;

}

void child_run(int port, int cpus) {
    srv::TcpServer tserv(port, cpus);

    if (!tserv.Init()) {
        std::cerr << "Initialize the tcp server failed" << std::endl;
        return;
    }

    std::thread t(std::bind(&srv::TcpServer::Start, &tserv));
    t.join();
} 

int main(int argc, char **argv) {
    //evthread_pool_test();

    if (argc != 2) {
        std::cerr << "Usage: tser <port>" << std::endl;
        return -1;
    }

    int port = std::atoi(argv[1]);
    if (port <= 0 || port >= 65535) {
        std::cerr << "port must between 1 - 65535" << std::endl;
        return -1;
    }

    auto cpus = sysconf(_SC_NPROCESSORS_ONLN);
    cpus = cpus > 4 ? cpus : 4;

    int pid = fork();
    if (pid < 0) {
        std::cerr << "Create child process failed" << std::endl;
        return -1;
    }

    if (pid == 0) {
        child_run(port, cpus);
        exit(0);
    }

    waitpid(pid, NULL, 0);    

    return 0;
}
