
#include "Command.h"
#include <iostream>
#include<vector>
#include<iostream>
#include "ThreadPool.h"
#include "Servo.h"

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


WheelCommand::WheelCommand(ThreadPool & pool):m_pool(pool)
{
	m_servo = new exploringBB::Servo(std::string(PWM_OUT_22));

}

void WheelCommand::Execute()
{
	std::cout << "Reach SpoonCommand Command" << std::endl;
	m_servo->Move_to(ServoValue);
	/*std::this_thread::sleep_for(std::chrono::seconds(2));
	m_servo->Move_to(30);
	std::this_thread::sleep_for(std::chrono::seconds(2));
	m_servo->Move_to(60);
	std::this_thread::sleep_for(std::chrono::seconds(2));
	m_servo->Move_to(90);
	std::this_thread::sleep_for(std::chrono::seconds(2));
	m_servo->Move_to(170);*/
}

void WheelCommand::setCommand(WheelMessage *  msg)
{
	ServoValue = msg->ServoValue;
}
