#include "cGame.h"
#include "cMap.h"
#include "cLevel.h"

cGame::cGame(void)
{
	//Czekaj na po³¹czenie 2 graczy
	return;
	//Start gry
	//this->start();
	this->chosen_map = 1;

	while(true){
		//Czekaj na po³¹czenie graczy
		this->waitForPlayers();

		//Stworzenie poziomu
		this->lvl = new cLevel();

		//Dodanie graczy
		for(int i=0;i<N_OF_PLAYERS;i++)
			lvl->addPlayer(players[i]);
		
		//Ustawienie wybranej mapy
		lvl->setMap(new cMap(chosen_map));

		//Start gry
		lvl->start();

		delete lvl;
	}
}

cGame::~cGame(void)
{
	for(int i=0;i<N_OF_PLAYERS;i++)
		if(players[i] != NULL) delete players[i];
}

void cGame::waitForPlayers(){
	//takie tam lobby do czekania na po³¹czenie siê wszystkich graczy jo³
}

//---Sprawdza, czy podany string znajduje sie w drugim stringu od danego miejsca
bool cGame::sprawdzString(string gdzie, string szukaj, int poz){
	if(poz<0) return false;

	int d1=gdzie.length();
	int d2=szukaj.length();

	//jezeli szukany napis nie miesci sie w stringu funkcji
	if(d1-poz<d2) return false;

	int j=0;
	int i;
	for(i=poz; i<d1 && j<d2; i++, j++)
		if(gdzie[i]!=szukaj[j]) return false;
	
	poz = i-1;
	return true;
};

//---Odebranie komunikatow od klienta
bool cGame::odbierzDane(string dane, connection *c, int &dana, int &n_of_conn){
	REQUESTS com = (REQUESTS)dane[0];

	//wyslanie odpowiedniej odpowiedzi na zapytanie
	switch(com){
		case ILOSC_GRACZY:{
			//printf("Ile graczy\n");
			string odp = "";
			odp+=ILOSC_GRACZY;
			odp+=to_string(long double(n_of_conn));	
			send(c->ClientSocket, odp.c_str(), odp.length()+1, 0 );
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
		default:
			printf("Odebralem zle polecenie: %s!\n",dane.c_str());
	}

	return true;
};