#include "cGame.h"
#include "cMap.h"
#include "cLevel.h"

cGame::cGame(void)
{
	//Czekaj na po³¹czenie 2 graczy
	//return;
	//Start gry
	//this->start();
	this->chosen_map = 1;
	numberOfPlayersToStart = 1;
	numberOfPlayers = 0;

	for(int i=0; i<N_OF_PLAYERS; i++) players[i] = NULL;
	//while(true){
		//Czekaj na po³¹czenie graczy
	//	this->waitForPlayers();

		//Stworzenie poziomu
		this->lvl = new cLevel();

		//Dodanie graczy
		for(int i=0;i<N_OF_PLAYERS;i++)
			lvl->addPlayer(players[i]);
		
		//Ustawienie wybranej mapy
	//	lvl->setMap(new cMap(chosen_map));

		//Start gry
	//	lvl->start();

	//	delete lvl;
	//}
}

cGame::~cGame(void)
{
	for(int i=0;i<N_OF_PLAYERS;i++)
		if(players[i] != NULL) delete players[i];
}

void cGame::waitForPlayers(){
	//takie tam lobby do czekania na po³¹czenie siê wszystkich graczy jo³
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

			//poinformowanie gracza o przydzielonym mu ID
			//sendToClient(c->ClientSocket, SET_PLAYER_ID, to_string(long double(id)));
			
			//poinformowanie innych graczy o dolaczeniu nowego pro gamera
			//for(int i=0; i<N_OF_PLAYERS; i++){
				//if(players[i]!=NULL){
					//sendToClient(c->ClientSocket, PLAYER_JOINED, nick+"\t"+to_string(long double(id)));

					//jest wystarczajaco duzo graczy do rozpoczecia rozgrywki
					if(numberOfPlayers>=numberOfPlayersToStart){
						printf("MOZNA ROZPOCZAC GRE!\n");
						sendToClient(c->ClientSocket, START_GAME);
					}
				//}
			//}

			printf("Dolaczyl %s o id %d!\n",nick.c_str(), id);
		break;}
		case PLAYER_QUIT:
			numberOfPlayers--;
		break;
		default:
			printf("Odebralem zle polecenie: %s!\n",dane.c_str());
	}

	return true;
};