#pragma once
#include<thread>
#include<vector>
#include <queue> 
#include <mutex>
#include <array>
class ThreadPool;

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
};


enum class RobotCommand
{
	GO = 0,
	BACK = 1,
	LEFT = 2,
	RIGHT = 3
};

class ParseCommand :public ICommand
{
public:
	ParseCommand(ThreadPool & pool,	 std::string command, int n);
	virtual void Execute();
private:
	ThreadPool& m_Pool;
	RobotCommand command;
//	std::array<std::string, 4> commands{ { "GO","BACK","LEFT","RIGHT" } };
};

/*
other components susscribe to this interface 
should implement subscribe event function
*/
class TimerCommand : public ICommand
{

};


