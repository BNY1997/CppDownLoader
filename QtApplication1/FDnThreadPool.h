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
			std::function<void()> func; // �������������func

			bool dequeued; // �Ƿ�����ȡ��������Ԫ��
			//
			while (m_pThreadPool->m_bShutdown == false || m_pThreadPool->m_queue.empty() == false)
			{
				{
					// Ϊ�̻߳��������������ʹ����̵߳����ߺͻ���
					std::unique_lock<std::mutex> lock(m_pThreadPool->m_conditional_mutex);

					// ����������Ϊ�գ�������ǰ�߳�
					if (m_pThreadPool->m_queue.empty())
					{
						m_pThreadPool->m_conditional_lock.wait(lock); // �ȴ���������֪ͨ�������߳�
					}

					// ȡ����������е�Ԫ��
					dequeued = m_pThreadPool->m_queue.dequeue(func);
				}

				// ����ɹ�ȡ����ִ�й�������
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
		m_conditional_lock.notify_all(); // ֪ͨ���������й����߳�

		for (int i = 0; i < m_threads.size(); ++i)
		{
			if (m_threads.at(i).joinable()) // �ж��߳��Ƿ��ڵȴ�
			{
				m_threads.at(i).join(); // ���̼߳��뵽�ȴ�����
			}
		}
	}
private:
	int m_nThreadNum;
	bool m_bShutdown; // �̳߳��Ƿ�ر�
	TaskQueue m_queue; // ִ�к�����ȫ���У����������
	vector<thread> m_threads; // �����̶߳���
	std::mutex m_conditional_mutex; // �߳��������������
	std::condition_variable m_conditional_lock; // �̻߳��������������̴߳������߻��߻���״̬

};



