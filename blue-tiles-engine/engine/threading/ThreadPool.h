#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <atomic>

class ThreadPool
{
public:

	ThreadPool(unsigned int threadCount);

	~ThreadPool();

	// Adds a task to the queue.
	// Example
	// ==============
	// void MyClass::My_Task(int x);
	//
	// AddTask(std::bind(&MyClass::My_Task, this, 10));
	void AddTask(std::function<void(void)> taskFunc);

	// Waits for all task to be completed.
	// Blocks the thread that calls this until all are completed.
	void WaitForAllTask();

private:

	// Function used for processing task.
	void ThreadFunc_Process();

	// Vector of threads.
	std::vector <std::thread*> m_threads;

	// Mutex for accessing the queue
	std::mutex m_queueMutex;

	// Queue condition_variable used for waiting for task.
	std::condition_variable m_queueCV;

	// A queue of function pointers
	std::queue<std::function <void(void)>> m_taskQueue;

	// Flag to kill all threads
	bool m_killFlag = false;

	// condition_variable used for blocking threads using WaitForAllTask.
	std::condition_variable m_waitCV;

	// Number of threads that are processing a task.
	std::atomic<int> m_workingThreads = 0;
};