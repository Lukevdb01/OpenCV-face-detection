#pragma once

#include <Windows.h>
#include <string>

class Serial
{
public:
	~Serial();

	void Open(std::string port, DWORD baudRate);
	void Write(int data_sent);
private:
	HANDLE hSerial;
	DCB dcbSerialParams = { 0 };
	COMMTIMEOUTS timeouts = { 0 };
};

