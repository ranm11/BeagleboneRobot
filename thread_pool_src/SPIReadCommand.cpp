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
	this Function should read from spi channel and enqueue Icommand to queue in an endless loop
*/
#define MAX_SPI_MESSAGE 38


SPIReadCommand::SPIReadCommand(ThreadPool & pool, SpiCommandSimulator* spi_sim) :m_Pool(pool), m_parseCommand(pool),m_spi_sim(spi_sim)
{
	m_spiDevice = new exploringBB::SPIDevice(0, 1);
}


void SPIReadCommand::Execute()
{
	std::cout << "Reach SPI Read Exe" << std::endl;
	uint8_t send_u[] = {
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0x40, 0x00, 0x00, 0x00, 0x00, 0x95,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xDE, 0xAD, 0xBE, 0xEF, 0xBA, 0xAD,
		0xF0, 0x0D,
	};
	unsigned char send[MAX_SPI_MESSAGE], receive[MAX_SPI_MESSAGE];
	m_spi_sim->getCommand(send);
	m_spiDevice->transfer(send, receive, MAX_SPI_MESSAGE);

	for (int ret = 0; ret < MAX_SPI_MESSAGE; ret++) {
		if (!(ret % 6))
			puts("");
		printf("%.2X ", receive[ret]);
	}
	puts("");
	// search for message header make sure its completed by length  call parsecommand here 
	// send connand after header
	m_parseCommand.Parse((char*)receive);

}