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

NetworkUdpReadCommand::NetworkUdpReadCommand(ThreadPool & pool):m_Pool(pool)
{

}


void NetworkUdpReadCommand::Execute()
{
	
	while (true)
	{
		std::cout << "pending UDP packet" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(5));

		char buffer[10];
		int n(10);
		ICommand*  cmd = new ParseCommand(m_Pool, buffer, n);
		m_Pool.Enque(cmd);
	}
	

}