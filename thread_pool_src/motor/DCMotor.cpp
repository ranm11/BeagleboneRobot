/*
 * DCMotor.cpp  Created on: 12 Jun 2014
 * Copyright (c) 2014 Derek Molloy (www.derekmolloy.ie)
 * Made available for the book "Exploring BeagleBone" 
 * See: www.exploringbeaglebone.com
 * Licensed under the EUPL V.1.1
 *
 * This Software is provided to You under the terms of the European 
 * Union Public License (the "EUPL") version 1.1 as published by the 
 * European Union. Any use of this Software, other than as authorized 
 * under this License is strictly prohibited (to the extent such use 
 * is covered by a right of the copyright holder of this Software).
 * 
 * This Software is provided under the License on an "AS IS" basis and 
 * without warranties of any kind concerning the Software, including 
 * without limitation merchantability, fitness for a particular purpose, 
 * absence of defects or errors, accuracy, and non-infringement of 
 * intellectual property rights other than copyright. This disclaimer 
 * of warranty is an essential part of the License and a condition for 
 * the grant of any rights to this Software.
 * 
 * For more details, see http://www.derekmolloy.ie/
 */

#include "DCMotor.h"

namespace exploringBB {

DCMotor::DCMotor(PWM *pwm, GPIO *gpio, GPIO *gpio2) {
	init(pwm, gpio, gpio2, CLOCKWISE, DEFAULT_DCMOTOR_SPEED);
}

DCMotor::DCMotor(PWM *pwm, int gpioNumber, int gpioNumber2) {
	this->gpio = new GPIO(gpioNumber);
	this->gpio_2 = new GPIO(gpioNumber2);
	this->gpio->setDirection(GPIO::OUTPUT);
	this->gpio_2->setDirection(GPIO::OUTPUT);
	init(pwm, this->gpio,this->gpio_2, CLOCKWISE, DEFAULT_DCMOTOR_SPEED);
}

DCMotor::DCMotor(PWM *pwm, GPIO *gpio, GPIO *gpio2, DCMotor::DIRECTION direction) {
	init(pwm, gpio,gpio2, direction, DEFAULT_DCMOTOR_SPEED);
}

DCMotor::DCMotor(PWM *pwm, int gpioNumber, int gpioNumber2, DCMotor::DIRECTION direction) {
	this->gpio = new GPIO(gpioNumber);
	this->gpio_2 = new GPIO(gpioNumber2);
	this->gpio->setDirection(GPIO::OUTPUT);
	this->gpio_2->setDirection(GPIO::OUTPUT);
	init(pwm, this->gpio, this->gpio_2, direction, DEFAULT_DCMOTOR_SPEED);
}

DCMotor::DCMotor(PWM *pwm, GPIO *gpio, GPIO *gpio2, DCMotor::DIRECTION direction, float speedPercent) {
	init(pwm, gpio,gpio2 ,direction, speedPercent);
}

DCMotor::DCMotor(PWM *pwm, int gpioNumber, int gpioNumber2, DCMotor::DIRECTION direction, float speedPercent) {
	this->gpio = new GPIO(gpioNumber);
	this->gpio_2 = new GPIO(gpioNumber2);
	this->gpio->setDirection(GPIO::OUTPUT);
	this->gpio_2->setDirection(GPIO::OUTPUT);
	init(pwm, this->gpio, this->gpio_2, direction, speedPercent);
}

void DCMotor::init(PWM *pwm, GPIO *gpio, GPIO *gpio2, DCMotor::DIRECTION direction, float speedPercent){
	this->pwm = pwm;
	this->gpio = gpio;
	this->gpio_2 = gpio2;
	this->gpio->setDirection(GPIO::OUTPUT);
	this->gpio_2->setDirection(GPIO::OUTPUT);
	this->direction = direction;
	this->speedPercent = speedPercent;
	this->stop();
	this->pwm->setPolarity(PWM::ACTIVE_LOW);
	this->setDirection(this->direction);
	this->setDutyCyclePeriod(DEFAULT_DCMOTOR_PWM_PERIOD);
	this->setSpeedPercent(this->speedPercent);
}

void DCMotor::setSpeedPercent(float speedPercentage){
	this->pwm->setDutyCycle(speedPercentage);
	//this->speedPercent = speedPercentage;
	pwm->setEnable(true);
	//	pwm->setDutyCycle(float(1000000));
}

void DCMotor::setSpeedPercent(unsigned int speedPercentage) {
	this->pwm->setDutyCycle(speedPercentage);
	pwm->setEnable(true);
}

void DCMotor::setDirection(DIRECTION direction){
	
	switch (direction)
	{
	case CLOCKWISE:
		this->gpio->setValue(GPIO::HIGH);
		this->gpio_2->setValue(GPIO::LOW);
		this->direction = direction;
		break;
	case ANTICLOCKWISE:
		this->gpio->setValue(GPIO::LOW);
		this->gpio_2->setValue(GPIO::HIGH);
		this->direction = direction;
		break;
	case IDLE:
		this->gpio->setValue(GPIO::LOW);
		this->gpio_2->setValue(GPIO::LOW);
		this->direction = direction;
		pwm->setEnable(false);
		break;
	default:
		std::cout<<"ERROR: Invalid direction"<<std::endl;
	}
}

void DCMotor::reverseDirection(){
	if(this->direction == CLOCKWISE){
		this->setDirection(ANTICLOCKWISE);
	}
	else{
		this->setDirection(CLOCKWISE);
	}
}

void DCMotor::stop(){
	this->pwm->stop();
}

void DCMotor::go(){
	this->pwm->run();
}

void DCMotor::setDutyCyclePeriod(unsigned int period_ns){
	this->pwm->setPeriod(period_ns);
}

DCMotor::~DCMotor() {
	delete gpio;
}

} /* namespace exploringBB */
