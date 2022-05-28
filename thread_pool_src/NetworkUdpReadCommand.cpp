//#include "pch.h"
#include "Command.h"
#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include "ThreadPool.h"
#ifndef WIN32
#include "GPIO.h"
#include "PWM.h"
#include "util.h"

#endif // !WIN32

/*
	this Function should read from UDP channel and enqueue Icommand to queue in an endless loop
*/
#define PORT     8080
#define MAXLINE 1024

NetworkUdpReadCommand::NetworkUdpReadCommand(ThreadPool & pool):m_Pool(pool)
{
	std::cout << "NetworkUdpReadCommand::NetworkUdpReadCommand" << std::endl;
}


void NetworkUdpReadCommand::Execute()
{
	std::cout << "NetworkUdpReadCommand::Execute" << std::endl;
	int sockfd;
	char buffer[MAXLINE];
	char *hello = "Hello from server";
	struct sockaddr_in servaddr, cliaddr;

	// Creating socket file descriptor
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));

	// Filling server information
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);

	// Bind the socket with the server address
	if (bind(sockfd, (const struct sockaddr *)&servaddr,
		sizeof(servaddr)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	int len, n;

	len = sizeof(cliaddr);  //len is value/result
	while (true)
	{
		std::cout << "pending UDP packet" << std::endl;
		n = recvfrom(sockfd, (char *)buffer, MAXLINE,
			MSG_WAITALL, (struct sockaddr *) &cliaddr,
			(socklen_t*)&len);

		ICommand*  cmd = new ParseCommand(m_Pool, buffer, n);
#ifndef WIN32
	//	using namespace exploringBB;
	//	exploringBB::GPIO outGPIO(45);
	//	outGPIO.printGpioPath();
	//	outGPIO.setDirection(GPIO::OUTPUT);
	//	std::cout << "GPIO(45) has value: " << outGPIO.getValue() << std::endl;
	//	outGPIO.setValue(GPIO::HIGH);
	//	//
	//	std::this_thread::sleep_for(std::chrono::seconds(2));
	//	std::cout << "GPIO(45) has value: " << outGPIO.getValue() << std::endl;
	//	outGPIO.setValue(GPIO::LOW);
	//	std::cout << "GPIO(44) has value: " << outGPIO.getValue() << std::endl;
#endif // !1
		m_Pool.Enque(cmd);
	}
	

}