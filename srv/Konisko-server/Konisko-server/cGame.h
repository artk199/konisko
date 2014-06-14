#pragma once
#include <cstdio>
#include <cstdlib>
#include <string>
#include <winsock2.h>
#include <process.h> 

class cLevel;
class cPlayer;

#define N_OF_PLAYERS 4

using namespace std;

struct connection{
	char nazwa[128];
	int id;
	SOCKET SendSocket;
	SOCKET RecieveSocket;
	HANDLE watek;
	string message;
	HANDLE send_message;
	int dana;
};

enum REQUESTS{ILOSC_GRACZY=1, KONIEC=2, DELTA=3, PLAYER_POSITION=4, UPDATE_ALL=5, START_GAME=6, SET_PLAYER_ID = 7,
	PLAYER_JOINED=8, PLAYER_QUIT=9, KEY_PRESSED=10};

class cGame{
	public:
		cGame();
		~cGame();

		bool odbierzDane(string dane, connection *c, int &dana, int &n_of_conn);//---Odebranie komunikatow od klienta
		void waitForPlayers();
		void loadPlayers();
		void sendToClient(connection* c, REQUESTS q, string par="");
		void send_data();
		int numberOfPlayersToStart;
	private:
		cPlayer* players[N_OF_PLAYERS];
		cLevel* lvl;
		int chosen_map;
		
		int numberOfPlayers;

		HANDLE amount_of_players_reached;
		HANDLE all_players_ready;
		HANDLE wyslij_delte;
		friend void __cdecl manageGame( void * Args ); // - Watek zarzadzajacy gra
		friend void __cdecl cos_do( void * Args ); // - Watek zarzadzajacy gra
};

