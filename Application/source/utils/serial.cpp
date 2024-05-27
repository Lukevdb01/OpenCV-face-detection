#include "serial.h"

Serial::~Serial()
{
	CloseHandle(hSerial);
}

void Serial::Open(std::string port, DWORD baudRate)
{
	hSerial = CreateFileA(port.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hSerial == INVALID_HANDLE_VALUE) {
		if (GetLastError() == ERROR_FILE_NOT_FOUND) {
			MessageBox(NULL, L"Serial port does not exist", L"Error", MB_OK | MB_ICONERROR);
		}
		MessageBox(NULL, L"Failed to open serial port", L"Error", MB_OK | MB_ICONERROR);
	}

	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	if (!GetCommState(hSerial, &dcbSerialParams)) {
		MessageBox(NULL, L"Failed to get comm state", L"Error", MB_OK | MB_ICONERROR);
	}

	dcbSerialParams.BaudRate = baudRate;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;

	if (!SetCommState(hSerial, &dcbSerialParams)) {
		MessageBox(NULL, L"Failed to set comm state", L"Error", MB_OK | MB_ICONERROR);
	}

	timeouts.ReadIntervalTimeout = 50;
	timeouts.ReadTotalTimeoutConstant = 50;
	timeouts.ReadTotalTimeoutMultiplier = 10;
	timeouts.WriteTotalTimeoutConstant = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;

	if (!SetCommTimeouts(hSerial, &timeouts)) {
		MessageBox(NULL, L"Failed to set comm timeouts", L"Error", MB_OK | MB_ICONERROR);
	}
}

void Serial::Write(int data_sent)
{	
	std::string data_sent_str = std::to_string(data_sent);
	DWORD bytes_written;
	unsigned int data_sent_length = static_cast<unsigned int>(data_sent_str.length());

	if (!WriteFile(hSerial, data_sent_str.c_str(), data_sent_length, &bytes_written, NULL)) {
		MessageBox(NULL, L"Failed to write to serial port", L"Error", MB_OK | MB_ICONERROR);
		CloseHandle(hSerial);
	}

}

