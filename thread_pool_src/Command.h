#pragma once
#include<thread>
#include<vector>
#include <queue> 
#include <mutex>
#include <array>
#include "ParseCommand.h"
#include"DCMotor.h"
#include "Servo.h"
#include "bus/SPIDevice.h"
#include "Messages.h"
//#include "rf24/RF24.h"
class ThreadPool;
class ParseCommand;
class RF24;

enum class SPI_IN_MODE :uint8_t
{
	SIMULATOR,
	ANALOG_A1_IN,
	ANALOG_A0_IN,
	RF24
};


class ICommand
{
public:
	virtual void Execute() = 0;
	virtual void NotifyEvent() = 0;

};

class TestCommand : public ICommand
{
public:
	virtual void Execute();
	virtual void NotifyEvent() {}
};

class NetworkUdpReadCommand :public ICommand
{
public:
	NetworkUdpReadCommand(ThreadPool & pool);
	virtual void Execute();
	virtual void NotifyEvent() {}
private:
	ThreadPool& m_Pool;
	ParseCommand m_parseCommand;
};

class ISpiInput
{
public:
	ISpiInput() {};
	virtual int getCommand(unsigned char* command) { return 0;}
protected:
	WheelMessage m_wheelMessage;
	EngineMessage m_engineMessage;

};

// SPI Command Simulator
class SpiCommandSimulator :public ISpiInput
{
public:
	SpiCommandSimulator();
	int getCommand(unsigned char* command);

};

class AnalogReadStick :public ISpiInput
{
public:
	AnalogReadStick();
	int getCommand(unsigned char* command);
private:
	std::string analogRead();
	//void autoConfig();
};

class SPIReadCommand :public ICommand
{
public:
	SPIReadCommand(ThreadPool & pool, SPI_IN_MODE _in_mode );
	virtual void Execute();
	virtual void NotifyEvent() {}
private:
	ThreadPool& m_Pool;
	ParseCommand m_parseCommand;
	exploringBB::SPIDevice* m_spiDevice;
	ISpiInput* m_spi_sim;
};

/*
other components susscribe to this interface 
should implement subscribe event function
*/
class TimerCommand : public ICommand
{

};

/*
	Medial command
*/
class EngineCommand : public ICommand
{
public:
	EngineCommand(ThreadPool & pool);
	virtual void Execute();
	virtual void NotifyEvent() {}
	void setCommand(EngineMessage * msg);
private:
	ThreadPool & m_pool;
	exploringBB::DCMotor * m_dcmotor;
	exploringBB::PWM * m_pwm;
	uint16_t direction;
	uint32_t speed;
};

/*
	Spoon command 
*/
class SpoonCommand : public ICommand
{
public:
	SpoonCommand(ThreadPool & pool);
	virtual void Execute();
	virtual void NotifyEvent() {}
	void setCommand(std::string cmd);
private:
	ThreadPool & m_pool;
	exploringBB::Servo * m_servo;
};

/*
	Spoon command
*/
class WheelCommand : public ICommand
{
public:
	WheelCommand(ThreadPool & pool);
	virtual void Execute();
	virtual void NotifyEvent() {}
	void setCommand(WheelMessage* cmd);
private:
	ThreadPool & m_pool;
	exploringBB::Servo * m_servo;
	uint16_t ServoValue;
};

/*
	RF24 Reader
*/

class RF24ReadCommand : public ICommand
{
public:
	RF24ReadCommand(ThreadPool &);
	virtual void Execute();
	virtual void NotifyEvent() {}
private:
	ThreadPool & m_pool;
	RF24* m_rf24;
};