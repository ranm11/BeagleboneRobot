
#include "Command.h"
#include <iostream>
#include<vector>
#include<iostream>
#include "ThreadPool.h"
#include"DCMotor.h"

constexpr int GPIO_OUT_44 = 44;
constexpr int GPIO_OUT_45 = 45;
constexpr char PWM_OUT_23[] = "pwm-7:1";
constexpr char PWM_OUT_22[] = "pwm-7\:0";

enum class opcode
{
	Medial,
	Lateral,
	Spoon
};

enum class Medial
{
	Forward,
	Backward
};

enum class Lateral
{
	Left,
	Right
};

/* | OPCODE 1 dword | Operation 1 DWord |Value 1 Dword */


EngineCommand::EngineCommand(ThreadPool & pool):m_pool(pool)
{
	//std::cout << "ParseCommand::ParseCommand" << std::endl;

	// commands =  {"GO","BACK","LEFT","RIGHT"} ;

}

void EngineCommand::Execute()
{
	std::cout << "Reach Medial Command" << std::endl;
	
}

void EngineCommand::setCommand(std::string cmd)
{

}
