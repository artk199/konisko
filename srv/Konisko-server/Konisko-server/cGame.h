#pragma once
#include <cstdio>
#include <cstdlib>
#include <string>
#include <winsock2.h>
#include <process.h> 
#include <map>

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

enum REQUESTS{ILOSC_GRACZY=33, KONIEC=34, DELTA=35, PLAYER_POSITION=36, UPDATE_ALL=37, START_GAME=38, SET_PLAYER_ID = 39,
	PLAYER_JOINED=40, PLAYER_QUIT=41, KEY_PRESSED=42, CONNECT=43, DESTROY=44};

class cGame{
	public:
		cGame();
		~cGame();

		bool odbierzDane(string dane, sockaddr_in c);//---Odebranie komunikatow od klienta
		void waitForPlayers();
		void loadPlayers();
		void sendToClient(connection* c, REQUESTS q, string par="");
		void send_data();
		int numberOfPlayersToStart;		
		HANDLE wyslij_delte;

		map<sockaddr_in, int> clients;
		HANDLE send_message;
		string message;
		SOCKET s;

	private:
		cPlayer* players[N_OF_PLAYERS];
		cLevel* lvl;
		int chosen_map;
		
		int numberOfPlayers;

		HANDLE amount_of_players_reached;
		HANDLE all_players_ready;
		
		friend void __cdecl manageGame( void * Args ); // - Watek zarzadzajacy gra
		friend void __cdecl cos_do( void * Args ); // - Watek zarzadzajacy gra
};

