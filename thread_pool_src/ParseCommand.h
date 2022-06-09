#pragma once
#include<thread>
#include<vector>
#include <queue> 
#include <mutex>
#include <array>
#ifndef WIN32
#include "util.h"
#include"DCMotor.h"
#include "Servo.h"
#endif // !WIN32

class ThreadPool;

class ParseCommand 
{
public:
	ParseCommand(ThreadPool & pool);
	void Parse(char*);
private:
	ThreadPool& m_Pool;
	//has Engine Command
	//has Lateral Command
	// Has Spoon Command
	void bbbTests();
};
