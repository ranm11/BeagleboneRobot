#pragma once
#include<thread>
#include<vector>
#include <queue> 
#include <mutex>
#include <array>

class ThreadPool;
class SpoonCommand;
class EngineCommand;
class SPISendCommand;
class WheelCommand;
// make this class startegy for caterpiller or  wheel
class ParseCommand 
{
public:
	ParseCommand(ThreadPool & pool);
	~ParseCommand();
	void Parse(char*);
private:
	ThreadPool& m_Pool;
	EngineCommand * m_engine1;
	SpoonCommand * m_spoon;
	SPISendCommand * m_spi_send;
	WheelCommand * m_wheelCommand;
	//has Lateral Command
	// Has Spoon Command
	void bbbTests();
};
