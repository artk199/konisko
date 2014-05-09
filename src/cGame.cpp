#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include "cGame.h"
#include "cNotify.h"
#include "core/STDFileSystem.h"
#include <iostream>
#include <regex>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#ifdef __S3E__
#include "s3eKeyboard.h"
#endif
#include "cCheckBox.h"

using namespace oxygine;

//DECLARE_SMART is helper, it does forward declaration and declares intrusive_ptr typedef for your class


file::STDFileSystem extfs(true);
cNotify * cGame::notifies;

//---Konstruktor, ustawia wartosci poczatkowe dla klas glownych
cGame::cGame(){
	//Wczytanie assetów
	Assets::load();

	notifies = new cNotify;
	//ustawienia wyswietlania informacji na ekranie
	memset(notifies->notifies, 0, sizeof(notifies->notifies));
	notifies->ui = new Actor;
	addChild(notifies->ui);

	setSize(getRoot()->getSize());
	delta = 0;

	//Dodanie gracza
	_player = new cPlayer(Vector2(0,0));
	_player->init(this);
	_player->setName(Assets::userNick);

	players.push_back(_player);

	Input::instance.addEventListener(Input::event_platform, CLOSURE(this, &cGame::_onPlatform));

	przes = 0;
};

//---Metody odpowiedzialne za po³¹czenie z serwerem
static DWORD WINAPI startSending(void* param){
    cGame* This = (cGame*) param;
    return This->sender();
}

static DWORD WINAPI startRecieving(void* param){
    cGame* This = (cGame*) param;
    return This->reciever();
}

DWORD cGame::sender(){   
	int iResult;
	while(1){
		WaitForSingleObject(send_message, INFINITE);
		char sendbuf[255];
		if (przes != 0){
			sprintf (sendbuf, "%d", przes);
			przes = 0;
		}else{
			sprintf (sendbuf, "daj");
		}
 		iResult = send( ConnectSocket, sendbuf, (int)strlen(sendbuf)+1, 0 );
		if (iResult == SOCKET_ERROR) {
			printf("blad podczas wysylania, koncze: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			return 1;
		}
		//std::cout<<"ping: "<<ping<<endl;
		ResetEvent(send_message);
	}
	return 0;
}
DWORD cGame::reciever()
{   
	int iResult;
	char buf[80];
	while (recv (ConnectSocket, buf, 80, 0) > 0)
	{
		if (strcmp(buf, "KONIEC") == 0)
		{
			printf("koncze polaczenie\n");
			return 0;
		}
		_player->move(Vector2(atoi(buf)*2,atoi(buf)*2));
		
		//printf("\n%d",atoi(buf));
	};
	return 0;
}
bool cGame::connectToServer(){	   
	WSADATA wsaData;
    struct addrinfo *result = NULL, *ptr = NULL, hints;
    int iResult;

	WSAStartup(MAKEWORD(2,2), &wsaData);
    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
	getaddrinfo(Assets::serverName.c_str(), "27015", &hints, &result);

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {
        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return false;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return false;
    }
	send_message=CreateEvent(0,1,0,0); /*No security descriptor, Manual Reset, initially 0, no name*/
	DWORD ThreadID;
	CreateThread(NULL, 0, startSending, (void*) this, 0, &ThreadID);
	CreateThread(NULL, 0, startRecieving, (void*) this, 0, &ThreadID);
	return true;
};

void cGame::disconnect(){
	closesocket(ConnectSocket);
	WSACleanup();
}

//---Funkcja ustawia wartosci poczatkowe dla klasy
void cGame::init(){
	disconnect();
	//dodanie dzieci
	level = new cLevel();
	menu = new cMenu(this);
	menu->setLevel(level);
	level->setMenu(menu);
	level->setGame(this);
	addChild(menu);
	addChild(level);
	level->setPlayers(&players);
}

//---Metody odpowiedzialne za ob³ugê klawiatury
void cGame::_onPlatform(Event *event){
		_onSDLEvent((SDL_Event*)event->userData);
};

int cGame::_onSDLEvent(SDL_Event *event){
	switch(event->type){
	case SDL_KEYDOWN:
		switch( event->key.keysym.sym ){
            case SDLK_LEFT:
                przes-=1;
            break;
            case SDLK_RIGHT:
                przes+=1;
            break;
            case SDLK_UP:
				przes+=1;
            break;
            case SDLK_DOWN:
                przes-=1;
            break;
            default:;
		}
		SetEvent(send_message);
		break;
	}
	return 0;
};

//---Funkcja czysci pamiec po zakonczeniu zycia klasy
void cGame::destroy(){
	Assets::free();
	delete notifies;
	this->disconnect();
};

//---Funkcja aktualizujaca czynnosci klasy
void cGame::doUpdate(const UpdateState &us){
	delta += us.dt;
	if (delta > 2000){
		delta = 0;
	}
	SetEvent(send_message);
};

//---Zwraca informacje, czy podana nazwa serwera jest poprawna (IP lub localhost)
bool cGame::checkServerName(string n){
	regex e ("[0-9][0-9]?[0-9]?\.[0-9][0-9]?[0-9]?\.[0-9][0-9]?[0-9]?\.[0-9][0-9]?[0-9]?");

	if(n=="localhost") return true;
	else if(regex_match(n,e)) return true; 
	else return false;
}; 

//---Zwraca informacje, czy udalo sie polaczyc do serwera (rzuca wyjatkiem, jesli nie)
bool cGame::tryConnectToServer(){
	//sprawdzenie poprawnosci nazwy
	if(!checkServerName(Assets::serverName)){
		throw 0;
		return false;	
	}
	//sprawdzenie mozliwosci polaczenia sie z serwerem
	if(!connectToServer()){
		throw 1;
		return false;
	}
	return true;
}; 