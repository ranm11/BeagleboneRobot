//#include "pch.h"
#include "Command.h"
std::mutex printMutex;
void TestCommand::Execute()
{
	static int nCount = 0;
	printf("\nExecuting:...");
	std::this_thread::sleep_for(std::chrono::seconds(10));
	printMutex.lock();
	printf("\nFinished:%d", ++nCount);
	printMutex.unlock();
}