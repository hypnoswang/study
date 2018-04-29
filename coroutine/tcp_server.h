#ifndef TCP_SERVER_h
#define TCP_SERVER_h


#include <thread>
#include <vector>
#include <atomic>

#include "thread_pool.h"
#include "evthread_pool.h"


namespace srv {


	class TcpServer {
		public:
			TcpServer(int port, std::size_t lc);
			virtual ~TcpServer();

			bool Init();
			bool Start();
			void Stop();
            void Handle(int fd, std::thread::id& dispatcher);

		private:
			TcpServer(const TcpServer& srv) = delete;
            TcpServer& operator=(const TcpServer& srv) = delete;

            struct Listener {
                std::atomic<int>    stat_;
                std::shared_ptr<std::thread>        th_;

                Listener() {
                    stat_.store(0);
                    th_.reset();
                }
            };
            typedef std::shared_ptr<Listener> ListenerPtr;

			void listen_main(ListenerPtr& t);
            void dispatch(int fd);
			void stop();

		private:
			int port_;

            std::size_t lc_;
			std::vector<ListenerPtr> listeners_;

            std::atomic<int>        stat_;
            //thp::ThreadPoolPtr      worker_pool_;
            evthp::EvThreadPoolPtr  worker_pool_;
            

	};

}


#endif 
