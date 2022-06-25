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

class SPIReadCommand :public ICommand
{
public:
	SPIReadCommand(ThreadPool & pool);
	virtual void Execute();
private:
	ThreadPool& m_Pool;
	
};

// SPI sender 
class SPISendCommand :public ICommand
{
public:
	SPISendCommand(ThreadPool & pool);
	virtual void Execute();
private:
	ThreadPool& m_Pool;
	ParseCommand m_parseCommand;
	exploringBB::SPIDevice* m_spiDevice;
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

