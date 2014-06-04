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



using namespace oxygine;


file::STDFileSystem extfs(true);
cNotify * cGame::notifies;

//---Konstruktor, ustawia wartosci poczatkowe dla klas glownych
cGame::cGame(){
	//Wczytanie assetów
	Assets::load();

	//ustawienia wyswietlania informacji na ekranie
	notifies = new cNotify;
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
		if(ConnectSocket == INVALID_SOCKET) break;
		
		//zapytanie o przesuniecie gracza
		if (przes != 0){
			askServer(Assets::PLAYER_POSITION, to_string(long double(przes)));
			przes = 0;
		}
		//pobieranie delty
		else
			askServer(Assets::DELTA);
	}
	return 0;
}

DWORD cGame::reciever(){   
	int iResult;
	char buf[80];
	while (recv (ConnectSocket, buf, 80, 0 ) > 0)
	{
		if(ConnectSocket == INVALID_SOCKET) printf("JESTEM w\n");

		Assets::REQUESTS com = (Assets::REQUESTS)buf[0];
		switch(com){
			case Assets::KONIEC:
				printf("koncze polaczenie\n");
				return 0;
			break;
			case Assets::ILOSC_GRACZY:{
				string par="";
				for(int i=1; i<strlen(buf); i++) par+=buf[i];
			break;}
			case Assets::DELTA:{
				string par="";
				for(int i=1; i<strlen(buf); i++) par+=buf[i];

				_player->move(Vector2(atoi(par.c_str())*2,atoi(par.c_str())*2));
			break;}
			
		}
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
	ConnectSocket = INVALID_SOCKET;
	WSACleanup();
}

//---Funkcja ustawia wartosci poczatkowe dla klasy
void cGame::init(){
	_masked = new MaskedSprite;
	_masked->attachTo(this);

	//dodanie dzieci
	level = new cLevel();
	menu = new cMenu(this, "sc_menu");
	menu->setLevel(level);
	level->setMenu(menu);
	level->setGame(this);
	
	menu->attachTo(_masked);
	level->attachTo(_masked);
	level->setPlayers(&players);

	//try{
	//	this->tryConnectToServer();
	//}
	//catch(...){}
	//printf("Odpowiedz: %s\n",this->askServer("testowe").c_str());
	/*EventCallback c;
	spActor t = new MaskedSprite;
	t->addChild(cUI::addButton(400,400,"test",c));

	t->attachTo(_masked);

	//test
	spSprite test = new Sprite;
	test->setResAnim(Assets::gameResources.getResAnim("anim"));
	test->attachTo(_masked);
	test->setScale(10);
	test->setPriority(-3000);

	spSprite _mask = new Sprite;
	_mask->setPriority(1);
	_mask->setResAnim(Assets::gameResources.getResAnim("anim"));
	_mask->setAlpha(0);
	
	_mask->setScale(4);
	_mask->setPosition(200,200);
	_mask->addTween(Actor::TweenScale(20), 1500, -1, true);

	addChild(_mask);
	_masked->setMask(_mask);*/
};

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

	spcButton b= menu->getChildT<cButton>("bt_options", oxygine::ep_ignore_error);
	
	//if(b!=NULL) b->onOff(false);


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

//---Wysyla zapytanie na serwer (wyslanie parametru opcjonalne)
void cGame::askServer(Assets::REQUESTS q, string parametr){
	//wyslanie zapytania
	string question = " ";
	question[0] = q;
	if(parametr!="brak") question+=parametr;

	WaitForSingleObject(send_message, 5);
 	int iResult = send( ConnectSocket, question.c_str(), question.length()+1, 0 );
	//sprawdzenie polaczenia z serwerem
	if (iResult == SOCKET_ERROR) {
		printf("blad podczas wysylania, koncze: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		//throw 2; 
	}
	ResetEvent(send_message);
};
