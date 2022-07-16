#pragma once

#define MAX_SPI_MESSAGE 38
constexpr uint16_t message_header = 0x5BFC;

typedef enum  
{
	Engine,
	Wheel,
	Spoon
}CommandType;

class IMessage
{
public:
	IMessage(uint8_t type) :header(message_header),commandType(type) {}
	uint16_t header;
	uint8_t commandType;
};

class WheelMessage :public IMessage
{
public:
	WheelMessage() :IMessage(uint8_t(Wheel)) {}
	uint16_t ServoValue;
};

class EngineMessage :public IMessage
{
public:
	EngineMessage() :IMessage(uint8_t(Engine)) {}
	uint16_t MotorSpeed;

};