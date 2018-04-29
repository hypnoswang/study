#ifndef EVTHREAD_POOL_H
#define EVTHREAD_POOL_H


#include <list>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <functional>

#include "event2/event.h"


namespace evthp {

    typedef std::function<void(void)> Job;

    class JobQueue {
        public:
            JobQueue(std::size_t mx);
            virtual ~JobQueue();

            bool    Push(const Job& j);
            Job     Pop();

        private:
            JobQueue(const JobQueue&) = delete;
            JobQueue(JobQueue&&) = delete;
            JobQueue& operator=(const JobQueue&) = delete;
            JobQueue& operator=(JobQueue&&) = delete;

        private:
            std::size_t     max_;

            std::timed_mutex    tmtx_;
            std::list<Job>      jobs_;
    };
    typedef std::unique_ptr<JobQueue> JobQueuePtr;

    class EvThread;
    typedef std::unique_ptr<EvThread> EvThreadPtr; 

    class EvThreadPool {
        public:
            EvThreadPool(std::size_t tc);
            virtual ~EvThreadPool();

            bool Init();
            bool Start();
            void Stop();

            bool Dispatch(Job j);

        private:
            EvThreadPool(const EvThreadPool&) = delete;
            EvThreadPool(EvThreadPool&&) = delete;
            EvThreadPool& operator=(const EvThreadPool&) = delete;
            EvThreadPool& operator=(EvThreadPool&&) = delete;

            void start();
            void stop();

        private:
            std::size_t tc_;
            std::size_t seq_;

            // 0 - created
            // 1 - initialized
            // 2 - running
            // 3 - stoping
            std::atomic_int         stat_;

            std::vector<EvThreadPtr> workers_; 

    };

    typedef std::unique_ptr<EvThreadPool> EvThreadPoolPtr;

    class EvThread {
        public:
            EvThread(std::size_t mx);
            virtual ~EvThread();

            bool Start();
            void Stop();

            bool AddJob(const Job& j);
            bool Ping();

            inline int* GetCh() { return ch_; }
            inline JobQueuePtr& GetQueue() { return queue_; }
            inline struct event_base* GetEvBase() { return eb_; }

            static void EvHandle(int fd, short what, void *arg);
            static void EvHandlePing(EvThread* etp);
            static void EvHandleJob(EvThread* etp);
            static void EvHandleStop(EvThread* etp);

        private:
            EvThread(const EvThread&) = delete;
            EvThread(const EvThread&&) = delete;
            EvThread& operator=(const EvThread&) = delete;
            EvThread& operator=(const EvThread&&) = delete;

            void threadMain();

            void stop();

        private:
            std::size_t max_;

            int         ch_[2];
            JobQueuePtr queue_;
            struct event_base *eb_;
            std::unique_ptr<std::thread> worker_;
    };




}





#endif
