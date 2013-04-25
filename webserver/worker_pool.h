#ifndef WORKER_POOL_H
#define WORKER_POOL_H

/*#ifdef _WIN32
	#include <windows.h>
	#include "webserver/winthread.h"
#else
	#include <pthread.h>
#endif*/

#include "concurrency/thread.h"
#include "concurrency/mutex.h"
#include "concurrency/condition_variable.h"
#include <vector>
#include <deque>
#include <iostream>

namespace webserver
{
	template<class T>
	class worker_pool
	{
		private:
			int workers_number;
			int active_workers; /* inactive_workers=workers_number-active_workers */
			
			concurrency::condition_variable cond;
			concurrency::mutex mutx;
			std::deque<T> job_queue;
			
			std::vector<concurrency::thread*> workers;	
			void* handler;
			
		public:
			worker_pool(int k) : workers_number(k)
			{
				
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
				concurrency::scoped_lock l(mutx);
				job_queue.push_back (job);
				
				cond.notify_one();
			}
			
			T get_job ()
			{
				concurrency::scoped_lock l(mutx);
				while (job_queue.size() == 0)
				{
					cond.wait (mutx);
				}
			
				T job = job_queue.front();
				job_queue.pop_front();
				// unlock mutex so other worker threads may access the queue
				
				return job;
			}
	};
};

#endif
