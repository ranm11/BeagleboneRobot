
#include "../Command.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

#define CAPE "cape-bone-iio"

#define AIN1 "/sys/devices/platform/ocp/44e0d000.tscadc/TI-am335x-adc.0.auto/iio\:device0/in_voltage1_raw"

using namespace std;
/*
void AnalogReadStick::autoConfig()
{
	ofstream myFile;
	myFile.open(PATH, ios::in);
	myFile << CAPE << endl;
	myFile.close();
}
*/
string AnalogReadStick::analogRead() // trigged by spi sim loop
{
	ifstream myFile0(AIN1);
	if (myFile0)
	{
		string ligne0;
		while (getline(myFile0, ligne0))
		{
			return ligne0;
		}
	}
}

int AnalogReadStick::getCommand(unsigned char* data_to_send)
{
	//string 
	//see whtat get here
	return 0;
}

AnalogReadStick::AnalogReadStick()
{
}
/*
int main()
{
	autoConfig();

	int ain0;
	while (1)
	{
		ain0 = atoi(analogRead().c_str());
		cout << ain0 << endl; //wanna plot this value on the lcdnumber of Qt
	}
	return 0;
}*/