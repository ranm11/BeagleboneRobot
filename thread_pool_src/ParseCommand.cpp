
#include "ParseCommand.h"
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
/*
 this unit should know if we have wheels or caterpiler track
 should instantiate 2 engines in case of caterpiller 
 or
 1 motor and 1 servo
 init with Factory
*/

ParseCommand::ParseCommand(ThreadPool & pool):m_Pool(pool)
{
	m_engine1 = new EngineCommand(pool);
	m_spoon = new SpoonCommand(pool);
	//if wheels - instantiate - if caterpiller instantiate 2 engines - have a startegy pattern 
	m_wheelCommand = new WheelCommand(pool);
	
}

ParseCommand::~ParseCommand()
{
	if (m_engine1 != nullptr)
	{
		delete m_engine1;
	}
	if (m_spoon != nullptr)
	{
		delete m_spoon;
	}
}
/*
	this function go over 
*/
void ParseCommand::Parse(char* command)
{
	//read type and length
	m_engine1->setCommand(std::string(command));
	m_spoon->setCommand(std::string(command));
	//m_Pool.Enque(m_engine1);
	//m_Pool.Enque(m_spoon);
}

void ParseCommand::bbbTests()
{
	std::cout << "Reach bbbTests" << std::endl;
#ifndef WIN32

//enqueue thread1 for power - will subscribe to ParseCommand Events provide notify function to ParseCommand
	using namespace exploringBB;
	exploringBB::PWM *pwm = new exploringBB::PWM(std::string(PWM_OUT_23));   // may be included in Motor 
	exploringBB::DCMotor * dcMotor = new exploringBB::DCMotor(pwm, GPIO_OUT_44, GPIO_OUT_45, DCMotor::IDLE);
	dcMotor->setSpeedPercent((unsigned int)50000);
	dcMotor->setDirection(DCMotor::CLOCKWISE);
	std::this_thread::sleep_for(std::chrono::seconds(2));
	dcMotor->setSpeedPercent((unsigned int)10000000);
	dcMotor->setDirection(DCMotor::ANTICLOCKWISE);
	std::this_thread::sleep_for(std::chrono::seconds(2));
	dcMotor->setDirection(DCMotor::IDLE);


	//enque thread2 for Servo- will subscribe to ParseCommand Events provide notify function to ParseCommand
	exploringBB::Servo* servo = new exploringBB::Servo(std::string(PWM_OUT_22));
	servo->Move_to(0);
	std::this_thread::sleep_for(std::chrono::seconds(2));
	servo->Move_to(30);
	std::this_thread::sleep_for(std::chrono::seconds(2));
	servo->Move_to(60);
	std::this_thread::sleep_for(std::chrono::seconds(2));
	servo->Move_to(90);
	std::this_thread::sleep_for(std::chrono::seconds(2));
	servo->Move_to(170);
	//std::cout << "GPIO("<< GPIO_PORT <<") has value: " << outGPIO.getValue() << std::endl;
#endif // !1
	
}
