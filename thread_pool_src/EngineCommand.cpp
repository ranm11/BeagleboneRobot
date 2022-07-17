
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
	m_pwm = new exploringBB::PWM(std::string(PWM_OUT_23));   // may be included in Motor 
	m_dcmotor = new exploringBB::DCMotor(m_pwm, GPIO_OUT_44, GPIO_OUT_45, exploringBB::DCMotor::IDLE);

}

void EngineCommand::Execute()
{
	std::cout << "Reach Medial Command" << std::endl;

	m_dcmotor->setSpeedPercent((unsigned int)speed);
	m_dcmotor->setDirection(exploringBB::DCMotor::CLOCKWISE);
	std::this_thread::sleep_for(std::chrono::seconds(2));
	m_dcmotor->setSpeedPercent((unsigned int)10000000);
	m_dcmotor->setDirection(exploringBB::DCMotor::ANTICLOCKWISE);
	std::this_thread::sleep_for(std::chrono::seconds(2));
	m_dcmotor->setDirection(exploringBB::DCMotor::IDLE);
	
}

void EngineCommand::setCommand(EngineMessage * msg)
{
	speed = msg->MotorSpeed;
}
