
#include "Command.h"
#include <iostream>
#include<vector>
#include<iostream>
#include "ThreadPool.h"
#ifndef WIN32
//#include "GPIO.h"
//#include "PWM.h"
#include "util.h"
#include"DCMotor.h"
#include "Servo.h"
#endif // !WIN32

constexpr int GPIO_OUT_44 = 44;
constexpr int GPIO_OUT_45 = 45;
constexpr char PWM_OUT_23[] = "pwm-7:1";
constexpr char PWM_OUT_22[] = "pwm-7\:0";

ParseCommand::ParseCommand(ThreadPool & pool, std::string command ,int size):m_Pool(pool)
{
	//std::cout << "ParseCommand::ParseCommand" << std::endl;

	// commands =  {"GO","BACK","LEFT","RIGHT"} ;

}

void ParseCommand::Execute()
{
	std::cout << "Reach ParseConnand" << std::endl;
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
