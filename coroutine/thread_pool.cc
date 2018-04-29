#include <ios>
#include <iostream>
#include <sstream>
#include "thread_pool.h"



namespace thp {

    Thread::Thread(std::string& name, ThreadPool* tp):name_(name), tp_(tp) {
        flogger_.open(name+".log", std::ios_base::out|std::ios_base::ate);
        stat_.store(0);
    }

    Thread::~Thread() {
        flogger_.close();
    }

    bool Thread::Start() {
        thread_ = new std::thread(std::bind(&Thread::thread_main, this));
        if (thread_ == NULL) return false;

        return true;
    }

    void Thread::Stop() {
        if (stat_.load() == 1) {
            stat_.store(2);
            thread_->join();
            stat_.store(0);
        }
    }

    void Thread::thread_main() {
        if (stat_.load() != 0) return;

        stat_.store(1);


        //ThreadPool* tp = GetPool();
        //std::mutex& mtx = tp->GetMutex();
        //std::condition_variable& cond = tp->GetCondVar();
        //JobQueue& queue = tp->GetQueue();

        //std::thread::id tid = std::this_thread::get_id();

        std::size_t counter = 0;
        Job j;
        for (;;) {
            if (stat_.load() == 2) break;   // the thread is stopped

            std::chrono::milliseconds to(100);
            std::unique_lock<std::mutex> lock(mtx_);
            bool ok = cond_.wait_for(lock, to, [this]{ return !queue_.empty(); });
            if (ok) {
                j = queue_.front();
                queue_.pop_front();
            }
            lock.unlock();

            if (ok) {
                counter ++;
                // flogger_ << "[" << tid << "]: Get job " << counter << std::endl;
                j();
            }
        }
    }

    ThreadPool::ThreadPool(std::size_t max, std::size_t tc) :
        max_(max), stat_(0), seq_(0), tc_(tc) {
    }

    ThreadPool::~ThreadPool() {
        stop();
        //TODO stop all the threads
    }

    bool ThreadPool::Init() {
        if (max_ <= 0) return false;
        if (tc_ <= 0 || tc_ > 24) return false;

        for (std::size_t i = 0; i < tc_; ++i) {
            std::ostringstream oss;
            oss << "thread_" << i;
            std::string name = oss.str();
            ThreadPtr t(new Thread(name, this));
            if (!t) {
                exit(-1);
            }

            ths_.push_back(t);
        }

        stat_ = 1;

        return true;
    }

    bool ThreadPool::Start() {
        for (std::size_t i = 0; i < tc_; ++i) {
            if (!ths_[i]->Start()) {
                for (std::size_t j = 0; j < i; ++j) {
                    ths_[j]->Stop();
                }
                return false;
            }
        }

        stat_ = 2;

        return true;
    }

    void ThreadPool::Stop() {
        stop();
    }

    void ThreadPool::stop() {
        // std::cerr << "ThreadPool.stop is being called" << std::endl;

        for (std::size_t i = 0; i < tc_; ++i) {
            ths_[i]->Stop();
        }

        stat_ = 0;
    }

    bool ThreadPool::AddJob(Job j) {
        //std::unique_lock<std::mutex> lock(mtx_); // will be unlocked automatically when destruct
        //if (queue_.size() >= max_) { return false; }
        //queue_.push_back(j);
        //cond_.notify_one();

        for (std::size_t i = 1; i < tc_; ++i) {
            std::size_t idx = (seq_ + i) % tc_;
        
            ThreadPtr& t = ths_[idx];
            std::unique_lock<std::mutex> lock(t->GetMutex()); // will be unlocked automatically when destruct
            if (t->GetQueue().size() >= max_) {
                lock.unlock();
            } else {
                seq_ = idx;
                t->GetQueue().push_back(j);
                t->GetCondVar().notify_one();
                return true;
            }
        }

        return false;
    }


}
