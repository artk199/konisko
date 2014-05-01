#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include "cGame.h"
#include "oxygine-framework.h"
#include "Assets.h"
#include "Actor.h"
#include "Button.h"
#include "RenderState.h"
#include "core/STDFileSystem.h"
#include "cNotify.h"
#include "cUI.h"
#include <stdlib.h>
#include <iostream>
#include "cMenu.h"
#include "cLevel.h"
// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#ifdef __S3E__
#include "s3eKeyboard.h"
#endif

using namespace oxygine;

//DECLARE_SMART is helper, it does forward declaration and declares intrusive_ptr typedef for your class
DECLARE_SMART(cGame, spcGame);

file::STDFileSystem extfs(true);


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

	//Dodanie mapy 
	_map = new cMap(this);

	//Dodanie gracza
	_player = new cPlayer(Vector2(0,0));
	_player->init(this);
	

	Input::instance.addEventListener(Input::event_platform, CLOSURE(this, &cGame::_onPlatform));

	przes = 0;
	 

	//wystartowanie testowej aplikacji
	start();
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
void cGame::connectToServer()
{	   
	WSADATA wsaData;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    int iResult;


	WSAStartup(MAKEWORD(2,2), &wsaData);
    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
	getaddrinfo("localhost", "27015", &hints, &result);

	//getaddrinfo("192.168.0.14", "27015", &hints, &result);

	    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return;
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
        return;
    }
	send_message=CreateEvent(0,1,0,0); /*No security descriptor, Manual Reset, initially 0, no name*/
	DWORD ThreadID;
	CreateThread(NULL, 0, startSending, (void*) this, 0, &ThreadID);
	CreateThread(NULL, 0, startRecieving, (void*) this, 0, &ThreadID);
}
void cGame::disconnect()
{
	closesocket(ConnectSocket);
	WSACleanup();
}

//---Funkcja ustawia wartosci poczatkowe dla klasy
void cGame::init(){

	cout<<"TRWA £¥CZENIE Z SERWEREM"<<endl;
	this->connectToServer();
	cout<<"PO£¥CZONO"<<endl;

}

//---Metody odpowiedzialne za ob³ugê klawiatury
void cGame::_onPlatform(Event *event)
	{
		_onSDLEvent((SDL_Event*)event->userData);
	}
int cGame::_onSDLEvent(SDL_Event *event)
	{
		switch(event->type)
		{
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
                    default:
                        break;
                }
			SetEvent(send_message);
			break;
		}
	return 0;
}

//---Funkcja czysci pamiec po zakonczeniu zycia klasy
void cGame::destroy(){
	Assets::free();
	delete _map;
	delete _player;
	this->disconnect();
}

//---Funkcja aktualizujaca czynnosci klasy
void cGame::doUpdate(const UpdateState &us)
{
	delta += us.dt;
	if (delta > 1000){
		delta = 0;
		//displayClicked(NULL);
	}
	SetEvent(send_message);
}

//---************************************************FUNKCJE TESTOWE DLA APLIKACJI (Z SZABLONU)
void cGame::displayClicked(Event *event){
	//setVisible(false);
	notifies->notify("Jakies dluzsze informacje\n nawet w dwoch liniach Trolololololololololo");
	//lets create and run sprite with animation
	runSprite();
	SetEvent(send_message);

};

void cGame::start(){	
	level = new cLevel();
	menu = new cMenu();
	menu->setGame(level);
	level->setMenu(menu);
	addChild(menu);
	//EventCallback c = CLOSURE(this, &cGame::displayClicked);
	//content->addChild(cUI::addButton(400,400, "trololo", c));
	//content->addChild(cUI::addButton(400,500, "Trolololo2", c));
	
};

void cGame::runSprite(){
		spSprite sprite = new Sprite();		
		addChild(sprite);

		int duration = 500;//500 ms 
		int loops = -1;//infinity loops

		//animation has 7 columns, check res.xml
		ResAnim *animation = Assets::gameResources.getResAnim("tile1");

		//add animation tween to sprite
		//TweenAnim would change animation frames
		sprite->addTween(TweenAnim(animation), duration, loops);

		Vector2 destPos = getRoot()->getSize() - sprite->getSize();
		Vector2 srcPos = Vector2(0, destPos.y);
		//set sprite initial position
		sprite->setPosition(srcPos);		

		//add another tween: TweenQueue
		//TweenQueue is a collection of tweens
		spTweenQueue tweenQueue = new TweenQueue();
		tweenQueue->setDelay(1500);

		//first, move sprite to dest position
		tweenQueue->add(Sprite::TweenPosition(destPos), 1500, 1);
		//then fade it out smoothly
		tweenQueue->add(Sprite::TweenAlpha(0), 1000, 1);
		
		sprite->addTween(tweenQueue);

		//and remove sprite from tree when tweenQueue is empty
		//if you don't hold any references to sprite it would be deleted automatically
		tweenQueue->setDetachActor(true);		

};

