#pragma once
#include <cstdio>
#include <cstdlib>
#include <string>
#include <winsock2.h>

using namespace std;

struct connection{
	char nazwa[128];
	int id;
	SOCKET ClientSocket;
	HANDLE watek;
	int dana;
};

enum REQUESTS{ILOSC_GRACZY=1, KONIEC=2, DELTA=3, PLAYER_POSITION=4};

class cGame{
	public:
		cGame();
		~cGame();

		bool sprawdzString(string gdzie, string szukaj, int poz=0);
		bool odbierzDane(string dane, connection *c, int &dana, int &n_of_conn);//---Odebranie komunikatow od klienta
};

