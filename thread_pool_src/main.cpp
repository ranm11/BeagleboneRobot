// ThreadPool.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#include "pch.h"
#include <iostream>
#include<thread>
#include<vector>
#include <queue> 
#include <mutex>
//#include <conio.h>
#include "Worker.h"
#include "Command.h"
#include "ThreadPool.h"

enum class Configuration
{
	Caterpiller,
	Wheels,
	Spoon
};

int main()
{
    std::cout << "BeagleBone Robot\n"; 
	ThreadPool pool(8);
	pool.Start();
	ICommand * udp = new NetworkUdpReadCommand(pool);
	pool.Enque(udp);
	//ICommand * m_spi_send = new SPISendCommand(pool);
	//ICommand *spiChannel = new SPIReadCommand(pool,SPI_IN_MODE::ANALOG_A1_IN); //SIMULATOR/ STICK  add startegy here
	//pool.Enque(spiChannel);
	//ICommand * rf24ReadCommand = new RF24ReadCommand(pool);
	//pool.Enque(rf24ReadCommand);
	
}

