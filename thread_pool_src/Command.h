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

class ThreadPool;
class ParseCommand;

#define MAX_SPI_MESSAGE 38
constexpr uint16_t message_header = 0x5BFC;

enum class CommandType: std::uint8_t
{
	Engine,
	Wheel,
	Spoon
};

class ICommand
{
public:
	virtual void Execute() = 0;

};

class TestCommand : public ICommand
{
public:
	virtual void Execute();
};

class NetworkUdpReadCommand :public ICommand
{
public:
	NetworkUdpReadCommand(ThreadPool & pool);
	virtual void Execute();
private:
	ThreadPool& m_Pool;
	ParseCommand m_parseCommand;
};

// SPI Command Simulator
class SpiCommandSimulator
{
public:
	SpiCommandSimulator();
	int getCommand(unsigned char* command);
private:

};

class SPIReadCommand :public ICommand
{
public:
	SPIReadCommand(ThreadPool & pool , SpiCommandSimulator* spi_sim = nullptr);
	virtual void Execute();
private:
	ThreadPool& m_Pool;
	ParseCommand m_parseCommand;
	exploringBB::SPIDevice* m_spiDevice;
	SpiCommandSimulator* m_spi_sim;
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
	void setCommand(std::string cmd);
private:
	ThreadPool & m_pool;
	exploringBB::DCMotor * m_dcmotor;
	exploringBB::PWM * m_pwm;
};

/*
	Spoon command 
*/
class SpoonCommand : public ICommand
{
public:
	SpoonCommand(ThreadPool & pool);
	virtual void Execute();
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
	void setCommand(std::string cmd);
private:
	ThreadPool & m_pool;
	exploringBB::Servo * m_servo;
};
