#pragma once
#include<iostream>
#include<vector>
#include<utility>
#include<thread>
#include<functional>
#include<queue>
#include<mutex>
#include<condition_variable>
#include<random>
#include<future>
#include"SafeQueue.h"

using std::vector;
using std::thread;
using Function = std::function<void()>;
using uint = unsigned int;

class ThreadPool
{
public:
	using TaskQueue = SafeQueue<Function>;
	class Task
	{
		ThreadPool* m_pThreadPool;
		uint m_nId;
	public:

		Task(ThreadPool* threadPool, uint id) :
			m_pThreadPool(threadPool),
			m_nId(id)
		{

		}
		void operator() ()
		{
			std::function<void()> func; // 定义基础函数类func

			bool dequeued; // 是否正在取出队列中元素
			//
			while (m_pThreadPool->m_bShutdown == false || m_pThreadPool->m_queue.empty() == false)
			{
				{
					// 为线程环境加锁，互访问工作线程的休眠和唤醒
					std::unique_lock<std::mutex> lock(m_pThreadPool->m_conditional_mutex);

					// 如果任务队列为空，阻塞当前线程
					if (m_pThreadPool->m_queue.empty())
					{
						m_pThreadPool->m_conditional_lock.wait(lock); // 等待条件变量通知，开启线程
					}

					// 取出任务队列中的元素
					dequeued = m_pThreadPool->m_queue.dequeue(func);
				}

				// 如果成功取出，执行工作函数
				if (dequeued)
					func();
			}
		}


	};
	ThreadPool(int nThreadNum = 4)
		:m_nThreadNum(nThreadNum),
		m_bShutdown(false)
	{
		for (int i = 0; i < m_nThreadNum; i++)
		{
			m_threads.push_back(std::thread(Task(this, i)));
		}
	}
	~ThreadPool()
	{
		shutdown();
	}
	ThreadPool(const ThreadPool&) = delete;
	ThreadPool(ThreadPool&&) = delete;
	ThreadPool& operator = (const ThreadPool&) = delete;
	ThreadPool& operator = (ThreadPool&&) = delete;
	template<class Func, class ...Args>
	void AddTask(Func&& func, Args&&...args)
	{
		auto taskWork = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
		auto task_ptr = std::make_shared<std::packaged_task<decltype(func(args...))()>>(taskWork);
		// Warp packaged task into void function
		std::function<void()> warpper_func = [task_ptr]()
		{
			(*task_ptr)();
		};
		m_queue.enqueue(warpper_func);
		m_conditional_lock.notify_one();
	}
	// Waits until threads finish their current task and shutdowns the pool
	void shutdown()
	{
		if (m_bShutdown == true)
			return;
		m_bShutdown = true;
		m_conditional_lock.notify_all(); // 通知，唤醒所有工作线程

		for (int i = 0; i < m_threads.size(); ++i)
		{
			if (m_threads.at(i).joinable()) // 判断线程是否在等待
			{
				m_threads.at(i).join(); // 将线程加入到等待队列
			}
		}
	}
private:
	int m_nThreadNum;
	bool m_bShutdown; // 线程池是否关闭
	TaskQueue m_queue; // 执行函数安全队列，即任务队列
	vector<thread> m_threads; // 工作线程队列
	std::mutex m_conditional_mutex; // 线程休眠锁互斥变量
	std::condition_variable m_conditional_lock; // 线程环境锁，可以让线程处于休眠或者唤醒状态

};



