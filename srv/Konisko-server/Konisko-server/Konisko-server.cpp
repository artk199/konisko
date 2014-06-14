#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include "cGame.h"
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

#define BUFLEN 512
string DEFAULT_PORT = "27015";
#define MAX_CONN 5
int n_of_conn = 0;
int dana;

SOCKET s;
struct sockaddr_in server, si_other;
int slen , recv_len;
bool ready = false;

cGame game;

void __cdecl odbieraj(void *a){
	char buf[BUFLEN];
	//keep listening for data
    while(1)
    {
        printf("Waiting for data...");
        fflush(stdout);
         
        //clear the buffer by filling null, it might have previously received data
        memset(buf,'\0', BUFLEN);
         
        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == SOCKET_ERROR)
        {
            printf("recvfrom() failed with error code : %d" , WSAGetLastError());
            exit(EXIT_FAILURE);
        }

		game.odbierzDane(buf,si_other);
		//SetEvent(game.wyslij_delte);
         
        //print details of the client/peer and the data received
        printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        printf("Data: %s\n" , buf);

    }
}

void __cdecl wysylaj(void* x){
	//wyslanie zapytania
        /*//now reply the client with the same data
        if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == SOCKET_ERROR)
        {
            printf("sendto() failed with error code : %d" , WSAGetLastError());
            exit(EXIT_FAILURE);
        }/*/

	/*while(true){
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
	}*/
	return;
};

int __cdecl main(void) 
{
	std::cout<<"podaj port: ";
	std::cin>>DEFAULT_PORT;

    WSADATA wsa;
    slen = sizeof(si_other) ;
     

    //Initialise winsock
    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    printf("Initialised.\n");
    
	
    //Create a socket
    if((s = socket(AF_INET , SOCK_DGRAM , 0 )) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d" , WSAGetLastError());
    }
    printf("Socket created.\n");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( atoi(DEFAULT_PORT.c_str()) );

    //Bind
    if( bind(s ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d" , WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    puts("Bind done");
	

	HANDLE hThread =( HANDLE ) _beginthread(odbieraj, 0,0 );
	HANDLE hThread2 =( HANDLE ) _beginthread(wysylaj, 0,0 );

	while(1){}
    return 0;
}
