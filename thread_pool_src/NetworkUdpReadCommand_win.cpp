//#include "pch.h"
#include "Command.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "ThreadPool.h"
#include<thread>
#include<chrono>
/*
	this Function should read from UDP channel and enqueue Icommand to queue in an endless loop
*/
#define PORT     8080
#define MAXLINE 1024

NetworkUdpReadCommand::NetworkUdpReadCommand(ThreadPool & pool):m_Pool(pool),m_parseCommand(pool)
{
	
}


void NetworkUdpReadCommand::Execute()
{
	
	//while (true)
	//{
		std::cout << "pending UDP packet" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(3));

		char buffer[10];
		int n(10);
		//ICommand*  cmd = new ParseCommand(m_Pool, buffer, n);
		//ICommand* engine = new EngineCommand(m_Pool, buffer);
		//m_Pool.Enque(cmd);
		//m_parseCommand.Parse(buffer);
		ICommand * rf24_cmd = new RF24ReadCommand(m_Pool);
		m_Pool.Enque(rf24_cmd);
	//}
	

}