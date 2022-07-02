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
#include "Command.h"
/*
	this module replace the udp module
	it should hold the parse command
	it should invoke the spoon / engine commands

	spi read run in loop every X ms
	grab commands and send parseCommand for evey command read 
*/
/*

2 byte prefix  - 0x5BFC		
TAG - 1 byte
	SPOON
	Engine
Length 2 byte
Value up to 30 bytes
*/


SpiCommandSimulator::SpiCommandSimulator()
{
	
}

int SpiCommandSimulator::getCommand(unsigned char* command)
{
	unsigned char send_engine_command[] = {
		0x5B, 0xFC, 0x00, 0x00, 0x10, 0xFF,
		0x40, 0x00, 0x00, 0x00, 0x00, 0x95,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xDE, 0xAD, 0xBE, 0xEF, 0xBA, 0xAD,
		0xF0, 0x0D,
	};

	unsigned char send_wheel_command[] = {
		0x5B, 0xFC, 0x01, 0x00, 0x10, 0xFF,
		0x40, 0x00, 0x00, 0x00, 0x00, 0x95,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xDE, 0xAD, 0xBE, 0xEF, 0xBA, 0xAD,
		0xF0, 0x0D,
	};

	unsigned char send_Spoon_command[] = {
		0x5B, 0xFC, 0x02, 0x00, 0x10, 0xFF,
		0x40, 0x00, 0x00, 0x00, 0x00, 0x95,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xDE, 0xAD, 0xBE, 0xEF, 0xBA, 0xAD,
		0xF0, 0x0D,
	};

	memcpy(command, send_engine_command, MAX_SPI_MESSAGE);
	return MAX_SPI_MESSAGE;
}

