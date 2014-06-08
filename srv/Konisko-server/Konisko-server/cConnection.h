#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <string>

class cConnection
{
public:
	cConnection(void);
	~cConnection(void);
	void setSocket(SOCKET s);
	SOCKET getSocket();
	void send(std::string s);
	void recieveData();
private:
	SOCKET ClientSocket;
	HANDLE watek;
};

