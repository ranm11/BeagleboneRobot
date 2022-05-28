/*
 * Servo.cpp  Created on: 12 May 2014
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

#include "Servo.h"
#include<iostream>
namespace exploringBB {

Servo::Servo(string pinName): PWM(pinName)
{
	this->setPolarity(PWM::ACTIVE_LOW);

}

void Servo::Move_to(unsigned int angle)
{
	double angle_f = (double)angle;
	double duty_min = 83.5;
	double duty_max = 95.5;
	double duty_span = duty_max - duty_min;
	double duty = 100 - (angle_f / 180)*duty_span + duty_min;
	this->setEnable(true);
	std::cout << "duty is : " << duty*this->getPeriod() << std::endl;
	this->setDutyCycle((unsigned int)duty*this->getPeriod());
}

Servo::~Servo() {
	// TODO Auto-generated destructor stub
}

} /* namespace exploringBB */
