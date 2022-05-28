#pragma once
#include<vector>
#include <queue> 
#include <mutex>
#include <thread>
#include <iostream>
class ThreadPool;
class Worker
{
public:
	Worker(ThreadPool& pool) :m_Pool(pool), m_bBoolStatus(false), m_bRunning(true)
	{
		std::cout << "Worker::Worker" << std::endl;
		m_Thread = new std::thread(&Worker::ThreadFun, this);
	}

	void Join()
	{
		m_Thread->join();
	}

private:
	void ThreadFun();

	bool m_bBoolStatus;
	std::thread* m_Thread;
	volatile bool m_bRunning;
	ThreadPool& m_Pool;
};
