//#include "pch.h"
#include "Worker.h"
#include "Command.h"
#include "ThreadPool.h"

void Worker::ThreadFun()
{
	while (/*m_Pool.Running()*/true)
	{
		ICommand * job = nullptr;

		if (!m_Pool.GetQueue().empty()) {
			job = m_Pool.GetQueue().front();
			m_Pool.GetQueue().pop();
		}

		if (job)
		{
			std::cout << " Worker::ThreadFun" << std::endl;
			job->Execute();
		}
	}
}
