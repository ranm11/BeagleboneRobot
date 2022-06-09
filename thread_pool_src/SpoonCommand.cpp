
#include "Command.h"
#include <iostream>
#include<vector>
#include<iostream>
#include "ThreadPool.h"
#ifndef WIN32
#include "util.h"
#include"DCMotor.h"
#include "Servo.h"
#endif // !WIN32

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


SpoonCommand::SpoonCommand(ThreadPool & pool, std::string command ):m_pool(pool)
{
	//std::cout << "ParseCommand::ParseCommand" << std::endl;

	// commands =  {"GO","BACK","LEFT","RIGHT"} ;

}

void SpoonCommand::Execute()
{
	std::cout << "Reach SpoonCommand Command" << std::endl;
	
}

void SpoonCommand::setCommand(std::string cmd)
{

}
