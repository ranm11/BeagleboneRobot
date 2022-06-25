//#include "pch.h"
#include "Command.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "ThreadPool.h"
#include<thread>
#include<chrono>
#include "bus/SPIDevice.h"

/*
	this Function should read from UDP channel and enqueue Icommand to queue in an endless loop
*/
#define MAXLINE 1024

SPIReadCommand::SPIReadCommand(ThreadPool & pool):m_Pool(pool)
{
	
}


void SPIReadCommand::Execute()
{
	
	while (true)
	{
		std::cout << "pending SPI packet" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(5));

		char buffer[10];
		int n(10);
		
		//m_parseCommand.Parse(buffer);
	}
	

}