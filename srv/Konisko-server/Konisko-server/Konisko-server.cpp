#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include "cGame.h"
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
string DEFAULT_PORT = "27015";
#define MAX_CONN 5

int n_of_conn = 0;

int dana;

connection connections[MAX_CONN];
cGame game;

DWORD WINAPI odbieraj(void* a){
	connection* c = (connection*)a;
	char buf[80];
	c->dana = 0;

	while (recv (c->RecieveSocket, buf, 80, 0) > 0){
		game.odbierzDane(buf,c,dana,n_of_conn);
		SetEvent(game.wyslij_delte);
	}
	
	n_of_conn--;
	c->RecieveSocket = INVALID_SOCKET;
	c->SendSocket = INVALID_SOCKET;
	closesocket(c->RecieveSocket);
	closesocket(c->SendSocket);
	printf("ZAKONCZONO POLACZENIE %d\n",c->id);
	
	return 0;
}
DWORD WINAPI wysylaj(void* x){

	connection *c = (connection *) x;

	//wyslanie zapytania
	
	while(true){
		WaitForSingleObject(c->send_message, -1);
		string question = c->message;
		int iResult = send( c->SendSocket, question.c_str(), question.length()+1, 0 );
		//sprawdzenie polaczenia z serwerem
		if (iResult == SOCKET_ERROR) {
			printf("blad podczas wysylania, koncze: %d\n", WSAGetLastError());
			closesocket(c->SendSocket);
			closesocket(c->RecieveSocket);
			WSACleanup();
			//throw 2; 
		}
		c->message = "";
		ResetEvent(c->send_message);
	}
	return 0;
};
int __cdecl main(void) 
{
	std::cout<<"podaj port: ";
	std::cin>>DEFAULT_PORT;
	//std::cout<<"podaj ilosc graczy do startu: ";
	//std::cin>>game.numberOfPlayersToStart;
	for(int i=0;i<MAX_CONN;i++){
		connections[i].SendSocket = INVALID_SOCKET;
		connections[i].RecieveSocket = INVALID_SOCKET;
	}
	dana = 0;
    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;

    SOCKET SendSocket = INVALID_SOCKET;
	SOCKET RecieveSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT.c_str(), &hints, &result);

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
						DWORD id;
    // Accept a client socket
	while(1){
		RecieveSocket = accept(ListenSocket, NULL, NULL);
		SendSocket = accept(ListenSocket, NULL, NULL);
		if (SendSocket== INVALID_SOCKET ||  RecieveSocket == INVALID_SOCKET) {
			printf("accept failed with error: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}else{
			//Dodaje po³¹czenie
			if (n_of_conn == MAX_CONN){
				printf("Maxymalna liczba polaczen osiagnieta.\n");
				continue;
			}
			//Szukam wolnego miejsca w tablicy
			for (int i=0;i<MAX_CONN;i++){
				//Znaleziono, dodaje po³aczenie
				if(connections[i].RecieveSocket == INVALID_SOCKET){

					//Zwiêkszenie licznika;
					n_of_conn++;
					//Dodanie po³¹czenia
					connections[i].SendSocket = SendSocket;
					connections[i].RecieveSocket = RecieveSocket;
					connections[i].id = i;
					connections[i].send_message =  CreateEvent(NULL, false, false, NULL);
					printf("Polaczono! czekam na wiadomosci\n");

					connections[i].watek = CreateThread(
						NULL,						// atrybuty bezpieczeñstwa
						0,							// inicjalna wielkoœæ stosu
						odbieraj,					// funkcja w¹tku
						(void *)&connections[i],	// dane dla funkcji w¹tku
						0,							// flagi utworzenia
						&id );
					if( connections[i].watek != INVALID_HANDLE_VALUE )
					{ 
						printf( "Utworzylem watek o identyfikatorze %d\n",i);
						// ustawienie priorytetu
						SetThreadPriority( connections[i].watek, THREAD_PRIORITY_NORMAL );
					}

					connections[i].watek = CreateThread(
						NULL,						// atrybuty bezpieczeñstwa
						0,							// inicjalna wielkoœæ stosu
						wysylaj,					// funkcja w¹tku
						(void *)&connections[i],	// dane dla funkcji w¹tku
						0,							// flagi utworzenia
						&id );
					printf( "Hello world\n" );
					break;
				}

			}
			//Dodaje po³¹czenie
		}
	}
    // No longer need server socket
    closesocket(ListenSocket);

    WSACleanup();

    return 0;
}
