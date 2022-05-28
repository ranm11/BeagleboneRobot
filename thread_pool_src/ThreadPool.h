#pragma once
#include "BlockingQueue.h"
#include "Command.h"
#include "Worker.h"


class ThreadPool
{
public:
	ThreadPool(int nSize = std::thread::hardware_concurrency()) :m_nSize(nSize), m_bRunning(true)
	{
		
	}

	~ThreadPool()
	{
		ShutDown();
	}
	void Enque(ICommand * cmd);
	void Start();
	void ShutDown();
	volatile bool& Running();
	std::condition_variable& GetCondition();

private:
	BlockingQueue<ICommand *>& GetQueue();

	std::vector<Worker*> m_Workers;
	BlockingQueue<ICommand*> m_Jobs;
	volatile bool m_bRunning;
	std::mutex mtxThreadStatus;
	std::mutex mtxThreadJob;
	int m_nSize;
	std::condition_variable m_Cv;
	friend class Worker;
};
