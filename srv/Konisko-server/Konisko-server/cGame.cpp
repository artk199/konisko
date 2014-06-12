#include "cGame.h"
#include "cMap.h"
#include "cLevel.h"


//---Watek zarzadzajacy gra
// do poprawy: po rozlaczeniu sie gracza nalezy ponownie sprawdzac, czy wymagana ilosc graczy jest osiagnieta
// i jesli nie wracac do funkcji waitforplayers
void __cdecl manageGame( void * x ){
	cGame *game = (cGame *) x;

	//oczekiwanie na polaczenie sie odpowiedniej ilosci graczy
	game->waitForPlayers();
	game->loadPlayers();

	//Stworzenie poziomu
	game->lvl = new cLevel();
	
	//Dodanie graczy
	for(int i=0;i<game->numberOfPlayers;i++)
		game->lvl->addPlayer(game->players[i]);

	game->lvl->setMap(new cMap(game->chosen_map));

	game->lvl->start();

	delete game->lvl;

	//wyjscie z watku
	_endthread();
};


cGame::cGame(void)
{
	//Czekaj na po³¹czenie 2 graczy
	this->chosen_map = 1;
	numberOfPlayersToStart = 1;
	numberOfPlayers = 0;
	
	for(int i=0; i<N_OF_PLAYERS; i++) players[i] = NULL;

	amount_of_players_reached =  CreateEvent(NULL, false, false, NULL);

	//utworzenie watku zarzadzajacego gra
	HANDLE hThread =( HANDLE ) _beginthread(manageGame, 0, this );


}


cGame::~cGame(void)
{
	for(int i=0;i<N_OF_PLAYERS;i++)
		if(players[i] != NULL) delete players[i];
}

//---Oczekiwanie az polaczy sie wymagana ilosc graczy
void cGame::waitForPlayers(){

	WaitForSingleObject(amount_of_players_reached, -1);

	printf("Wymagana ilosc graczy do startu osiagnieta!\n");

}

//---Sprawdzanie po³¹czenia z graczami oraz wysy³anie informacji poczatkowych
void cGame::loadPlayers(){

	//poinformowanie wszystkich graczy, ze mozna rozpoczac gre
	for(int i=0; i<numberOfPlayers; i++)
		if(players[i]!=NULL)
			sendToClient(players[i]->getConnection()->getSocket(), START_GAME);	

	//Czekam 2s na to aby wszyscy gracze zglosili gotowosc w innym przypadku startuje
	//WaitForSingleObject(all_players_ready, 2000);

}

//---Wysyla do klienta odpowiedz z mozliwoscia dodania parametru
void cGame::sendToClient(SOCKET c, REQUESTS q, string par){

	//wyslanie zapytania
	string question = "";
	question += q;
	if(par!="") question+=par;

	WaitForSingleObject(send_message, 1000);
 	int iResult = send( c, question.c_str(), question.length()+1, 0 );
	//sprawdzenie polaczenia z serwerem
	if (iResult == SOCKET_ERROR) {
		printf("blad podczas wysylania, koncze: %d\n", WSAGetLastError());
		closesocket(c);
		WSACleanup();
		//throw 2; 
	}
	ResetEvent(send_message);
};

//---Odebranie komunikatow od klienta
bool cGame::odbierzDane(string dane, connection *c, int &dana, int &n_of_conn){
	REQUESTS com = (REQUESTS)dane[0];

	//wyslanie odpowiedniej odpowiedzi na zapytanie
	switch(com){
		case ILOSC_GRACZY:{
			sendToClient(c->ClientSocket, ILOSC_GRACZY, to_string(long double(n_of_conn)));
		break;}
		case KONIEC:
			printf("Po³¹czenie z graczem zakoñczone!\n");
			printf("Player %d roz³¹czony.!\n",c->id);
			n_of_conn--;
			closesocket(c->ClientSocket);
			return false;
		break;
		case DELTA:
			if(this->lvl != NULL){
				sendToClient(c->ClientSocket, DELTA, this->lvl->serialize());
				printf("%s\n",this->lvl->serialize());
			}
		break;
		case PLAYER_POSITION:{
			string pozycja="";
			for(int i=1; i<dane.length(); i++)  pozycja+=dane[i];
	
			dana+=atoi(pozycja.c_str());
		break;}
		/*case START_GAME:{
			players[c->id]->setReady(true);

			//Sprawdza czy wszyscy s¹ gotowi do gry
			bool all_ready = true;

			for(int i=0; i<numberOfPlayers; i++)
				if(players[i]!=NULL && !plyers[i]->isReady()){
					all_ready = false;		
					break;
				}

			if(all_ready)
				SetEvent(all_players_ready);

		break;}*/
		case PLAYER_JOINED:{
			//odczytanie parametru
			string nick="";
			for(int i=1; i<dane.length(); i++)  nick+=dane[i];
			int id = c->id;

			//dodanie nowego gracza
			players[id] = new cPlayer();
			players[id]->setNick(nick);
			players[id]->id = id;
			cConnection *conn = new cConnection();
			conn->setSocket(c->ClientSocket);
			players[id]->setConnection(conn);

			numberOfPlayers++;

			//poinformowanie gracza o przydzielonym mu ID
			sendToClient(c->ClientSocket, SET_PLAYER_ID, to_string(long double(id)));
			
			Sleep(10);

			//poinformowanie innych graczy o dolaczeniu nowego pro gamera
			for(int i=0; i<N_OF_PLAYERS; i++)
				if(players[i]!=NULL)
					sendToClient(players[i]->getConnection()->getSocket(), PLAYER_JOINED, nick+"\t"+to_string(long double(id)));
				
			printf("Dolaczyl %s o id %d (z %d)!\n",nick.c_str(), id, numberOfPlayers);
		
			if(numberOfPlayers >= numberOfPlayersToStart)
				SetEvent(amount_of_players_reached);
		break;}
		case PLAYER_QUIT:
			numberOfPlayers--;
		break;
		case KEY_PRESSED:{
			//Przeczytaæ kto to podes³a³ 
			E_DIRECTION dir;
			string pozycja="";
			for(int i=1; i<dane.length(); i++)  pozycja+=dane[i];
			switch(atoi(pozycja.c_str())){
			case 1:
				dir = LEFT;
				break;
			case 2:dir = RIGHT;
				break;
			case 3:dir = TOP;
				break;
			case 4:dir = BOT;
				break;
			}
			players[c->id]->changeDirection(dir);
			printf("KLAWISZ!\n");
			break;}
		default: printf("Odebralem zle polecenie: %s!\n",dane.c_str());
	}

	return true;
};