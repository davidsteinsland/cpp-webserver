#ifndef WORKER_POOL_H
#define WORKER_POOL_H

#ifdef _WIN32
	#include <windows.h>
	#include "webserver/winthread.h"
#else
	#include <pthread.h>
#endif

#include "concurrency/thread.h"
#include <vector>
#include <deque>

namespace webserver
{
	template<class T>
	class worker_pool
	{
		private:
			int workers_number;
			int active_workers; /* inactive_workers=workers_number-active_workers */
			
			pthread_cond_t condition;
			pthread_mutex_t mutex;
			std::deque<T> job_queue;
			
			std::vector<concurrency::thread*> workers;	
			void* handler;
			
		public:
			worker_pool(int k) : workers_number(k)
			{
				pthread_cond_init(&condition, 0);
				pthread_mutex_init(&mutex, 0);
			}
			
			~worker_pool()
			{
				while (job_queue.size() > 0)
				{
					delete job_queue.front();
					job_queue.pop_front();
				}
				
				for(int i = 0; i < workers_number; i++)
				{
					delete workers[i];
				}
			}
			
			bool start_workers(void (*handle) (concurrency::thread *))
			{
				for (int i = 0; i < workers_number; i++)
				{
					concurrency::thread* t = new concurrency::thread(handle, this);
					workers.push_back(t);
					t->start();
				}
				
				return true;
			}
			
			void add_job (T job)
			{
				pthread_mutex_lock(&mutex);
				
				job_queue.push_back (job);
				
				pthread_cond_signal(&condition);
				pthread_mutex_unlock(&mutex);
			}
			
			T get_job ()
			{
				pthread_mutex_lock(&mutex);
				while (job_queue.size() == 0)
					pthread_cond_wait(&condition, &mutex);
			
				T job = job_queue.front();
				job_queue.pop_front();
				// unlock mutex so other worker threads may access the queue
				pthread_mutex_unlock(&mutex);
				
				return job;
			}
	};
};

#endif
