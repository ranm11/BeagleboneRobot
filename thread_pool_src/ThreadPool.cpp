
#include <iostream>
#include<thread>
#include<vector>
#include <queue> 
#include <mutex>
#include "Worker.h"
#include "Command.h"
#include "ThreadPool.h"


void ThreadPool::Enque(ICommand * cmd)
{
	mtxThreadJob.lock();
	m_Jobs.push(cmd);
	mtxThreadJob.unlock();
	m_Cv.notify_one();
}

void ThreadPool::Start()
{
	for (int i = 0; i < m_nSize; i++)
	{
		Worker *pWorker = new Worker(*this);
		m_Workers.push_back(pWorker);

	}
}

void ThreadPool::ShutDown()
{
	{
		std::unique_lock<std::mutex> lock(mtxThreadStatus);
		m_bRunning = false;
	}
	m_Cv.notify_all();
	// Join all threads.
	for (Worker *worker : m_Workers)
	{
		worker->Join();
		delete worker;
	}

	m_Workers.clear();
}

volatile bool& ThreadPool::Running()
{
	return m_bRunning;
}

std::condition_variable& ThreadPool::GetCondition()
{
	return m_Cv;
}

BlockingQueue<ICommand *>& ThreadPool::GetQueue()
{
	return m_Jobs;
}