#include "ThreadPool.h"

// Based on David Schwartz's answer to https://stackoverflow.com/questions/26516683/reusing-thread-in-loop-c
ThreadPool::ThreadPool(unsigned int threadCount)
{
	// i slides to 0
	// https://stackoverflow.com/questions/1642028/what-is-the-operator-in-c
	unsigned int i = threadCount;
	while (i--\
			   \
			    \
				 \
				  \
				    > 0)
	{
		std::thread* thread = new std::thread(&ThreadPool::ThreadFunc_Process, this);

		m_threads.push_back(thread);
	}
}

ThreadPool::~ThreadPool()
{
	m_killFlag = true;

	for (int i = 0; i < m_threads.size(); i++)
	{
		m_threads[i]->join();
		delete m_threads[i];
	}
}

void ThreadPool::AddTask(std::function<void(void)> taskFunc)
{
	std::unique_lock<std::mutex> lk(m_queueMutex);

	m_taskQueue.emplace(std::move(taskFunc));

	m_queueCV.notify_one();
}

void ThreadPool::WaitForAllTask()
{
	std::unique_lock<std::mutex> lk(m_queueMutex);

	m_waitCV.wait(lk, [this] {
		return (m_killFlag || m_taskQueue.empty()) && m_workingThreads == 0;
	});
}

void ThreadPool::ThreadFunc_Process()
{
	std::function<void(void)> task;

	while (1)
	{
		std::unique_lock<std::mutex> lk(m_queueMutex);

		// wait for a task or kill flag
		m_queueCV.wait(lk, [this] { 
			return m_killFlag || !m_taskQueue.empty();
		});

		// kill thread if flag is set
		if (m_killFlag) return;

		m_workingThreads++;

		// get task
		task = std::move(m_taskQueue.front());
		m_taskQueue.pop();

		lk.unlock();

		// do task
		task();

		m_workingThreads--;
		
		// notify any threads waiting for task to be done
		m_waitCV.notify_all();
	}
}
