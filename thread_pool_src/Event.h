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
class ICommand;

class Observer
{
public:
	void attach(ICommand* _cmd)
	{
		command_vec.push_back(_cmd);
	}
protected:
	std::vector<ICommand*> command_vec;
};

class Event :public  ICommand ,public Observer
{
public:
	virtual void Execute()   //invoke notify for all subscribed
	{
		std::cout << "In Event Execute " << std::endl;
		for (auto & subscribed_cmd : command_vec)
		{
			std::cout << "In Loop subscribed vector " << std::endl;
			subscribed_cmd->NotifyEvent();//this
		}
	}
	void NotifyEvent() {}

};

class State :public Event
{
	//public void Execute();
};

class DMACall :public Event
{
	//public void Execute();
};

class Reset :public Event
{
	//public void Execute();
};