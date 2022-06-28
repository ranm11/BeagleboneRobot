/*
 * SPIDevice.cpp  Created on: 22 May 2014
 * Copyright (c) 2014 Derek Molloy (www.derekmolloy.ie)
 * Made available for the book "Exploring BeagleBone"
 * See: www.exploringbeaglebone.com
 * Licensed under the EUPL V.1.1
 *
 * This Software is provided to You under the terms of the European
 * Union Public License (the "EUPL") version 1.1 as published by the
 * European Union. Any use of this Software, other than as authorized
 * under this License is strictly prohibited (to the extent such use
 * is covered by a right of the copyright holder of this Software).
 *
 * This Software is provided under the License on an "AS IS" basis and
 * without warranties of any kind concerning the Software, including
 * without limitation merchantability, fitness for a particular purpose,
 * absence of defects or errors, accuracy, and non-infringement of
 * intellectual property rights other than copyright. This disclaimer
 * of warranty is an essential part of the License and a condition for
 * the grant of any rights to this Software.
 *
 * For more details, see http://www.derekmolloy.ie/
 */

#include "SPIDevice.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
using namespace std;

#define HEX(x) setw(2) << setfill('0') << hex << (int)(x)  //!< Macro for filling in leading 0 on HEX outputs

namespace exploringBB {

/**
 * The constructor for the SPIDevice that sets up and opens the SPI connection.
 * The destructor will close the SPI file connection.
 * @param bus The SPI bus number X (first digit after spidevX.Y)
 * @param device The device on the bus Y (second digit after spidevX.Y)
 */
SPIDevice::SPIDevice(unsigned int bus, unsigned int device):
	BusDevice(bus,device) {
	stringstream s;
	s << SPI_PATH << bus << "." << device;
	this->filename = string(s.str());
	this->mode = SPIDevice::MODE3;
	this->bits = 8;
	this->speed = 500000;
	this->delay = 0;
	this->open();
}

/**
 * This method opens the file connection to the SPI device.
 * @return 0 on a successful open of the file
 */
int SPIDevice::open(){
	//cout << "Opening the file: " << filename.c_str() << endl;
	
	if (this->setMode(this->mode)==-1) return -1;
	if (this->setSpeed(this->speed)==-1) return -1;
	if (this->setBitsPerWord(this->bits)==-1) return -1;
	return 0;
}

/**
 * Generic method to transfer data to and from the SPI device. It is used by the
 * following methods to read and write registers.
 * @param send The array of data to send to the SPI device
 * @param receive The array of data to receive from the SPI device
 * @param length The length of the array to send
 * @return -1 on failure
 */
int SPIDevice::transfer(unsigned char send[], unsigned char receive[], int length)
{
	memcpy(receive, send, length);
	return 0;
}

/**
 * A method to read a single register at the SPI address
 * @param registerAddress the address of the register from the device datasheet
 * @return the character that is returned from the address
 */
unsigned char SPIDevice::readRegister(unsigned int registerAddress){
	unsigned char send[2], receive[2];
	memset(send, 0, sizeof send);
	memset(receive, 0, sizeof receive);
	send[0] = (unsigned char) (0x80 | registerAddress);
	this->transfer(send, receive, 2);
	//cout << "The value that was received is: " << (int) receive[1] << endl;
	return receive[1];
}

/**
 * A method to read a number of registers as a data array
 * @param number the number of registers to read
 * @param fromAddress the starting address of the block of data
 * @return the data array that is returned (memory allocated by the method)
 */
unsigned char* SPIDevice::readRegisters(unsigned int number, unsigned int fromAddress){
	unsigned char* data = new unsigned char[number];
	
	return data;
}

/**
 *  A write method that writes a single character to the SPI bus
 *  @param value the value to write to the bus
 *  @return returns 0 if successful
 */
int SPIDevice::write(unsigned char value){
	unsigned char null_return = 0x00;
	//printf("[%02x]", value);
	this->transfer(&value, &null_return, 1);
	return 0;
}

/**
 *  A write method that writes a block of data of the length to the bus.
 *  @param value the array of data to write to the device
 *  @param length the length of the data array
 *  @return returns 0 if successful
 */
int SPIDevice::write(unsigned char value[], int length){
	unsigned char null_return = 0x00;
	this->transfer(value, &null_return, length);
	return 0;
}

/**
 *  Writes a value to a defined register address (check the datasheet for the device)
 *  @param registerAddress the address of the register to write to
 *  @param value the value to write to the register
 *  @return returns 0 if successful
 */
int SPIDevice::writeRegister(unsigned int registerAddress, unsigned char value){
	unsigned char send[2], receive[2];
	memset(receive, 0, sizeof receive);
	send[0] = (unsigned char) registerAddress;
	send[1] = value;
	//cout << "The value that was written is: " << (int) send[1] << endl;
	this->transfer(send, receive, 2);
	return 0;
}

/**
 *  A simple method to dump the registers to the standard output -- useful for debugging
 *  @param number the number of registers to dump
 */
void SPIDevice::debugDumpRegisters(unsigned int number){
	cout << "SPI Mode: " << this->mode << endl;
	cout << "Bits per word: " << (int)this->bits << endl;
	cout << "Max speed: " << this->speed << endl;
	cout << "Dumping Registers for Debug Purposes:" << endl;
	unsigned char *registers = this->readRegisters(number);
	for(int i=0; i<(int)number; i++){
		cout << HEX(*(registers+i)) << " ";
		if (i%16==15) cout << endl;
	}
	cout << dec;
}

/**
 *   Set the speed of the SPI bus
 *   @param speed the speed in Hz
 */
int SPIDevice::setSpeed(uint32_t speed){
	this->speed = speed;

	return 0;
}

/**
 *   Set the mode of the SPI bus
 *   @param mode the enumerated SPI mode
 */
int SPIDevice::setMode(SPIDevice::SPIMODE mode){
	this->mode = mode;

	return 0;
}

/**
 *   Set the number of bits per word of the SPI bus
 *   @param bits the number of bits per word
 */
int SPIDevice::setBitsPerWord(uint8_t bits){
	this->bits = bits;

	return 0;
}

/**
 *   Close the SPI device
 */
void SPIDevice::close(){
	this->file = -1;
}

/**
 *   The destructor closes the SPI bus device
 */
SPIDevice::~SPIDevice() {
	this->close();
}

} /* namespace exploringBB */
