#pragma once
#include <cstdio>
#include <cstdlib>
#include <string>
#include <winsock2.h>
#include "cPlayer.h"
#include <process.h> 

class cLevel;

#define N_OF_PLAYERS 4

using namespace std;

struct connection{
	char nazwa[128];
	int id;
	SOCKET ClientSocket;
	HANDLE watek;
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
		void sendToClient(SOCKET c, REQUESTS q, string par="");

	private:
		cPlayer* players[N_OF_PLAYERS];
		cLevel* lvl;
		int chosen_map;
		int numberOfPlayersToStart;
		int numberOfPlayers;

		HANDLE send_message;
		HANDLE amount_of_players_reached;
		HANDLE all_players_ready;

		friend void __cdecl manageGame( void * Args ); // - Watek zarzadzajacy gra
};

