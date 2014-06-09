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

	printf("JESTEM 1 \n");
	//Stworzenie poziomu
	game->lvl = new cLevel();
	printf("JESTEM 2 \n");
	//Dodanie graczy
	for(int i=0;i<game->numberOfPlayers;i++)
		game->lvl->addPlayer(game->players[i]);

	game->lvl->setMap(new cMap(game->chosen_map));

	game->lvl->start();

	printf("JESTEM 3 \n");
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

	//utworzenie watku zarzadzajacego gra
	HANDLE hThread =( HANDLE ) _beginthread(manageGame, 0, this );

		
		//Ustawienie wybranej mapy
	//	lvl->setMap(new cMap(chosen_map));

		//Start gry
	//	lvl->start();
}


cGame::~cGame(void)
{
	for(int i=0;i<N_OF_PLAYERS;i++)
		if(players[i] != NULL) delete players[i];
}

//---Oczekiwanie az polaczy sie wymagana ilosc graczy
void cGame::waitForPlayers(){
	while(numberOfPlayers <numberOfPlayersToStart){printf("");}

	//poinformowanie wszystkich graczy, ze mozna rozpoczac gre
	for(int i=0; i<numberOfPlayers; i++)
		if(players[i]!=NULL)
			sendToClient(players[i]->getConnection()->getSocket(), START_GAME);	

	printf("Wymagana ilosc graczy do startu osiagnieta!\n");
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
			//printf("Ile graczy\n");

			sendToClient(c->ClientSocket, ILOSC_GRACZY, to_string(long double(n_of_conn)));
		break;}
		case KONIEC:
			//printf("Koniec\n");
			printf("koncze polaczenie\n");
			n_of_conn--;
			closesocket(c->ClientSocket);
			return false;
		break;
		case DELTA:{
			//printf("Delta\n");
			string odp = "";
			odp+=DELTA;
			if(dana-c->dana != 0){
				odp+=to_string(long double(dana-c->dana));
				send(c->ClientSocket, odp.c_str(), odp.length()+1, 0 );
				c->dana = dana;
			}
		break;}
		case PLAYER_POSITION:{
			//printf("Pozycja\n");
			string pozycja="";
			for(int i=1; i<dane.length(); i++)  pozycja+=dane[i];
	
			dana+=atoi(pozycja.c_str());
		break;}
		case PLAYER_JOINED:{
			//odczytanie parametru
			string nick="";
			for(int i=1; i<dane.length(); i++)  nick+=dane[i];
			int id = c->id;

			//dodanie nowego gracza
			numberOfPlayers++;
			players[id] = new cPlayer();
			players[id]->setNick(nick);
			cConnection *conn = new cConnection();
			conn->setSocket(c->ClientSocket);
			players[id]->setConnection(conn);

			//poinformowanie gracza o przydzielonym mu ID
			sendToClient(c->ClientSocket, SET_PLAYER_ID, to_string(long double(id)));
			
			Sleep(10);

			//poinformowanie innych graczy o dolaczeniu nowego pro gamera
			for(int i=0; i<N_OF_PLAYERS; i++)
				if(players[i]!=NULL)
					sendToClient(players[i]->getConnection()->getSocket(), PLAYER_JOINED, nick+"\t"+to_string(long double(id)));
				
			printf("Dolaczyl %s o id %d (z %d)!\n",nick.c_str(), id, numberOfPlayers);
		break;}
		case PLAYER_QUIT:
			numberOfPlayers--;
		break;
		default:
			printf("Odebralem zle polecenie: %s!\n",dane.c_str());
	}

	return true;
};