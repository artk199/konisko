#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include "cGame.h"
#include "cNotify.h"
#include "core/STDFileSystem.h"
#include <iostream>
#include <regex>
#include <sstream>
#include <iostream>

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
	Assets::loadSettingsFromFile();

	//ustawienia wyswietlania informacji na ekranie
	notifies = new cNotify;
	memset(notifies->notifies, 0, sizeof(notifies->notifies));
	notifies->ui = new Actor;
	addChild(notifies->ui);

	setSize(getRoot()->getSize());
	delta = 0;

	Input::instance.addEventListener(Input::event_platform, CLOSURE(this, &cGame::_onPlatform));

	//dodanie graczy
	for(int i=0; i<4; i++) players.push_back(new cPlayer());

	klawisz = 0;

	przes = 0;

	this->num_of_players = 4;
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
		//gracz wcisnal klawisz
		if(klawisz!=0){
			askServer(Assets::KEY_PRESSED, to_string(long double(klawisz)));
			klawisz=0;
		}
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

//odebranie pakietow od serwera
DWORD cGame::reciever(){   
	int iResult;
	char buf[80];
	while (recv (ConnectSocket, buf, 80, 0 ) > 0){
		Assets::REQUESTS com = (Assets::REQUESTS)buf[0];
		//rozpoznanie typu komunikatu
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
				//cout<<par<<endl;
				parse_response(par);
			break;}
			case Assets::START_GAME:{
				printf("mozna startowac");
				//znalezienie przycisku umozliwiajacego dolaczenie do gry
				spcButton b=menu->getChildT<cButton>("bt_start", oxygine::ep_ignore_error);
				if(b!=NULL) b->onOff(true);

			break;}
			//nadanie graczowi ID przydzielonego z serwera
			case Assets::SET_PLAYER_ID:{ 
				//pobranie z parametru id gracza
				string par="";
				for(int i=1; i<strlen(buf); i++) par+=buf[i];
				int  id = atoi(par.c_str());

				printf("ID GRACZA USTAWIONO NA %d\n",id);
			break;}
			//dolaczyl nowy gracz, nalezy go dodac
			case Assets::PLAYER_JOINED:{
				//pobranie danych o graczu
				int i, ID;
				string nick, id;
				for(i=1; i<strlen(buf) && buf[i]!='\t'; i++) nick+=buf[i];
				for(i=i+1; i<strlen(buf) && buf[i]!='\t'; i++) id+=buf[i];
				ID=atoi(id.c_str());

				players[ID]->setNick(nick);
				players[ID]->setID(ID);

				int pozy=100+60*ID;
				int pozx=150;
				spTextActor napis=cUI::createText(players[ID]->getNick());
				napis->setPosition(pozx,pozy);
				napis->attachTo(menu);

				printf("Dolaczyl gracz %s - %s\n", nick.c_str(), id.c_str());
			break;}
			//nieznane polecenie
			default:
				printf("Nieprawidlowy komunikat: %s\n",buf);
		}
	};
	return 0;
}

void cGame::parse_response(string s){
	cSerializable* object = NULL;
	istringstream iss(s);
	istringstream iss2;
	cout<<s<<endl;
    do
    {
		string line,sub;
		getline(iss,line);
        iss2 = istringstream(line);
		iss2 >> sub;
		if(sub == "object"){
			iss2 >> sub;	
			object = NULL;
			if(sub == "player"){
				iss2 >> sub;
				int id = atoi(sub.c_str());
				if(id >= 0 && id < this->num_of_players)
					object = players[id].get();
			}
			continue;
		}
		if(object != NULL){
			if(line.length() != 0){
				object->deserialize(line);
				std::cout<<line<<"\n";
			}	
		}
    } while (iss);
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
	getaddrinfo(Assets::serverName.c_str(), Assets::serverPort.c_str(), &hints, &result);

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
	askServer(Assets::PLAYER_QUIT);
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
				klawisz=1;
            break;
            case SDLK_RIGHT:
                klawisz=2;
            break;
            case SDLK_UP:
				klawisz=3;
            break;
            case SDLK_DOWN:
                klawisz=4;
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
	Assets::saveSettingsToFile();

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

	//poinformowanie stanu gry na serwerze, ze dolaczyl gracz
	askServer(Assets::PLAYER_JOINED, Assets::userNick);

	return true;
}; 

//---Wysyla zapytanie na serwer (wyslanie parametru opcjonalne)
void cGame::askServer(Assets::REQUESTS q, string parametr){
	//wyslanie zapytania
	string question = "";
	question += q;
	if(parametr!="") question+=parametr;

	WaitForSingleObject(send_message, 1000);
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
