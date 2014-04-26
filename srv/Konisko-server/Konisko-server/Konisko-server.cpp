#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
#define MAX_CONN 5

int n_of_conn = 0;

struct connection{
	char nazwa[128];
	int id;
	SOCKET ClientSocket;
	HANDLE watek;
} connections[MAX_CONN];

DWORD WINAPI odbieraj(void* a){
	connection* c = (connection*)a;
	char buf [80];
	while (recv (c->ClientSocket, buf, 80, 0) > 0)
	{
		if (strcmp(buf, "KONIEC") == 0)
		{
			printf("koncze polaczenie\n");
			n_of_conn--;
			closesocket(c->ClientSocket);
			return 0;
		}

	};
	n_of_conn--;
	c->ClientSocket = INVALID_SOCKET;
	printf("ZAKONCZONO POLACZENIE %d\n",c->id);
	printf("\n%s",buf);
	return 0;
}
int __cdecl main(void) 
{
	for(int i=0;i<MAX_CONN;i++){
		connections[i].ClientSocket = INVALID_SOCKET;
	}

    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

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
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);

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
		ClientSocket = accept(ListenSocket, NULL, NULL);
		if (ClientSocket == INVALID_SOCKET) {
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
				if(connections[i].ClientSocket == INVALID_SOCKET){

					//Zwiêkszenie licznika;
					n_of_conn++;
					//Dodanie po³¹czenia
					connections[i].ClientSocket = ClientSocket;
					connections[i].id = i;
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
