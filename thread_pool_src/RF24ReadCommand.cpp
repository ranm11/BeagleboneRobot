
#include "Command.h"
#include <iostream>
#include "ThreadPool.h"
#include "rf24/RF24.h"

#define CE 7
#define CSN 8

RF24ReadCommand::RF24ReadCommand(ThreadPool & pool):m_pool(pool)
{
	m_rf24 = new RF24(CE, CSN);
}

void RF24ReadCommand::Execute()
{
	std::cout << "Reach RF24 Command Reader" << std::endl;
}

