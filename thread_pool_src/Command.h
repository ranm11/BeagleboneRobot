#pragma once
#include<thread>
#include<vector>
#include <queue> 
#include <mutex>
#include <array>
#include "ParseCommand.h"
#ifndef WIN32
#include "util.h"
#include"DCMotor.h"
#include "Servo.h"

#endif // !WIN32

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

// obsolete
/*enum class RobotCommand
{
	GO = 0,
	BACK = 1,
	LEFT = 2,
	RIGHT = 3
};*/

/*
this thread go over the input stream  , sort commands and enqueue relavant components
*/
/*class ParseCommand :public ICommand
{
public:
	ParseCommand(ThreadPool & pool,	 std::string command, int n);
	virtual void Execute();
private:
	ThreadPool& m_Pool;
	//RobotCommand command;
//	std::array<std::string, 4> commands{ { "GO","BACK","LEFT","RIGHT" } };
};*/

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
};

/*
	Spoon command
*/
class SpoonCommand : public ICommand
{
public:
	SpoonCommand(ThreadPool & pool, std::string command);
	virtual void Execute();
	void setCommand(std::string cmd);
private:
	ThreadPool & m_pool;
};

