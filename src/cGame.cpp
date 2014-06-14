#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include "cGame.h"
#include "cNotify.h"
#include "core/STDFileSystem.h"
#include <iostream>
#include <regex>
#include <sstream>
#include <iostream>

#include "cBomb.h"

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
	send_message = CreateEvent(NULL, false, false, NULL);
	message = "";
	this->num_of_players = 4;

	mainPlayerID=-1;


	//TO DO TESTU
	/*players[0]->setID(1);
	players[0]->setNick("Trolololo");
	players[0]->setPosition(100,100);
	players[0]->init();
	addChild(players[0]);
*/};

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
		if(SendSocket == INVALID_SOCKET) break;

		WaitForSingleObject(send_message, -1);
		string question = this->message;
		int iResult = send( SendSocket, question.c_str(), question.length()+1, 0 );
		//sprawdzenie polaczenia z serwerem
		if (iResult == SOCKET_ERROR) {
			printf("blad podczas wysylania, koncze: %d\n", WSAGetLastError());
			closesocket(SendSocket);
			closesocket(RecieveSocket);
			WSACleanup();
			//throw 2; 
		}
		//askServer(Assets::DELTA);
		ResetEvent(send_message);
	}
	return 0;
}

//odebranie pakietow od serwera
DWORD cGame::reciever(){   
	int iResult;
	char buf[512];
	while (recv (RecieveSocket, buf, 512, 0 ) > 0){
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

				mainPlayerID=id;
				printf("ID GRACZA USTAWIONO NA %d\n",id);
			break;}
			//dolaczyl nowy gracz, nalezy go dodac
			case Assets::PLAYER_JOINED:{
				int ile_graczy = buf[1] - '0';

				int i=2;
				//pobieranie nicka i id kolejnych graczy
				for(int n=0; n<ile_graczy; n++){
					string id, nick;
					for(i; i<strlen(buf) && buf[i]!='\t'; i++) nick+=buf[i];
					for(i=i+1; i<strlen(buf) && buf[i]!='\t'; i++) id+=buf[i];
					i++;

					int ID=atoi(id.c_str());
			
					// to jest nowy gracz
					if(players[ID]->getID()!=ID){
						players[ID]->setID(ID);
						players[ID]->setNick(nick);

						int pozy=100+60*ID;
						int pozx=150;

						bool edytowalny = false;
						if(ID==mainPlayerID) edytowalny = true;

						//dodanie nowego dziecka tylko jezeli nie zostalo juz dodane
						menu->addChild(new cCheckBox(pozx,pozy,&players[ID]->getReady(),players[ID]->getNick(),edytowalny,"player_"+to_string(long double(ID))+"_ready"));
						printf("Dodano gracza %s - %s\n", nick.c_str(), id.c_str());
					}	
				}		
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
				//std::cout<<line<<"\n";
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
        SendSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        RecieveSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (SendSocket == INVALID_SOCKET || RecieveSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return false;
        }

        // Connect to server.
        iResult = connect( SendSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        
		if (iResult == SOCKET_ERROR) {
            closesocket(SendSocket);
            SendSocket = INVALID_SOCKET;
            continue;
        }
        // Connect to server.
        iResult = connect( RecieveSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        
		if (iResult == SOCKET_ERROR) {
            closesocket(RecieveSocket);
            RecieveSocket = INVALID_SOCKET;
            continue;
        }
        
		break;
    }

    freeaddrinfo(result);

    if (SendSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return false;
    }
	send_message=CreateEvent(0,1,0,0); /*No security descriptor, Manual Reset, initially 0, no name*/
	DWORD ThreadID;
	DWORD ThreadID2;
	CreateThread(NULL, 0, startSending, (void*) this, 0, &ThreadID);
	CreateThread(NULL, 0, startRecieving, (void*) this, 0, &ThreadID2);
	return true;
};

void cGame::disconnect(){
	askServer(Assets::PLAYER_QUIT);
	closesocket(SendSocket);
	closesocket(RecieveSocket);
	SendSocket = INVALID_SOCKET;
	RecieveSocket = INVALID_SOCKET;
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
			case SDLK_SPACE:
                klawisz=5;
            break;
            default:;
		}
		this->askServer(Assets::KEY_PRESSED,std::to_string((long double)klawisz));
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

	//aktualizowanie bomb kazdego gracza
	for(int i=0; i<4; i++)
		if(players[i]->getID()>=0)
			players[i]->updateBombs(us.dt);
		
	
	if (delta > 10){
		delta = 0;
		askServer(Assets::DELTA);
		//SetEvent(send_message);
		//TO DO TESTU
		//players[0]->addBomb(0,300,300,4,1500);
		//players[0]->addBomb(1,500,500,4,1500);
	}
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
	this->message = question;
	SetEvent(send_message);
};
