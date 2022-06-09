// ThreadPool.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#include "pch.h"
#include <iostream>
#include<thread>
#include<vector>
#include <queue> 
#include <mutex>
//#include <conio.h>
#include "Worker.h"
#include "Command.h"
#include "ThreadPool.h"

enum class Configuration
{
	Caterpiller,
	Wheels,
	Spoon
};

int main()
{
    std::cout << "Hello World!\n"; 
	ThreadPool pool(8);
	pool.Start();
	ICommand * udp = new NetworkUdpReadCommand(pool);
	pool.Enque(udp);
	//for (int i = 0; i < 15; i++)
	//{
	//	ICommand * cmd = new TestCommand();
	//	pool.Enque(cmd);
	//}
	//
	//for (int i = 0; i < 15; i++)
	//{
	//	ICommand * cmd = new TestCommand();
	//	pool.Enque(cmd);
	//}

	//_getch();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
