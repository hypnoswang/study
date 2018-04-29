#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <atomic>
#include <vector>
#include <list>
#include <thread>
#include <functional>
#include <fstream>
#include <mutex>
#include <condition_variable>

namespace thp {

    typedef std::function<void(void)> Job;
    typedef std::list<Job> JobQueue;

    class ThreadPool;

    class Thread {
        public:
            Thread(std::string& name, ThreadPool* tp);
            virtual ~Thread();

            bool Start();
            void Stop();

            inline ThreadPool* GetPool() { return tp_; }
            inline std::ofstream& GetFLogger() { return flogger_; }

            inline std::mutex& GetMutex() { return mtx_; };
            inline std::condition_variable& GetCondVar() { return cond_; };
            inline JobQueue& GetQueue() { return queue_; }

        private:
            Thread(const Thread& th) = delete;
            Thread& operator=(const Thread& th) = delete;

            void thread_main();

        private:
            std::string         name_;
            std::ofstream       flogger_;

            std::mutex                      mtx_;
            std::condition_variable         cond_;
            JobQueue                        queue_;
            
            std::thread* thread_; 

            std::atomic<int>    stat_; // 0 - uninitialized 1 - running 2 - stopped
            ThreadPool*         tp_;
    };

    typedef std::shared_ptr<Thread> ThreadPtr;


    class ThreadPool {
        public:
            ThreadPool(std::size_t max, std::size_t tc);
            virtual ~ThreadPool();

            bool Init();
            bool Start();
            void Stop();

            bool AddJob(Job j);


            //inline std::mutex& GetMutex() { return mtx_; };
            //inline std::condition_variable& GetCondVar() { return cond_; };
            //inline JobQueue& GetQueue() { return queue_; }

        private:
            ThreadPool(const ThreadPool& thp) = delete;
            ThreadPool& operator=(const ThreadPool& thp) = delete;

            void thread_main();
            void stop();


        private:
            std::size_t     max_;
            int             stat_;  

            //std::mutex                      mtx_;
            //std::condition_variable         cond_;
            //JobQueue                        queue_;
            
            // the seq of the thread to which the last job was successfully dispatched
            std::size_t seq_;   
            std::size_t tc_;
            std::vector<ThreadPtr> ths_;
    };


	typedef std::shared_ptr<ThreadPool> ThreadPoolPtr;




}





#endif
