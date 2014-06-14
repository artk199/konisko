#pragma once
#include "Actor.h"
#include "Button.h"
#include "RenderState.h"
#include "cMap.h"
#include "cPlayer.h"
#include "oxygine-framework.h"
#include "Assets.h"
#include <ws2tcpip.h>
#include <windows.h>
#include <winsock2.h>
#include "cLevel.h"
#include "cMenu.h"
#include "cButton.h"
using namespace oxygine;

class cNotify;
typedef void (*handler) (Event);

DECLARE_SMART(cGame, spcGame);


class cGame: public Actor{
	public: 
		cGame();

		void init();		// - Funkcja ustawia wartosci poczatkowe dla klasy
		void destroy();		// - Funkcja czysci pamiec po zakonczeniu zycia klasy

		static cNotify *notifies;
		//Thread odpowiedzialny za wysy³anie operacji
		DWORD sender(void);
		DWORD reciever(void);

		bool checkServerName(string n); // - Zwraca informacje, czy podana nazwa serwera jest poprawna (IP lub localhost)
		bool tryConnectToServer(); // - Zwraca informacje, czy udalo sie polaczyc do serwera (rzuca wyjatkiem, jesli nie)
		void disconnect();		   // - Roz³¹czenie z serwerem
		void askServer(Assets::REQUESTS q, string parametr=""); // - Wysyla zapytanie na serwer (wyslanie parametru opcjonalne)
		void parse_response(string s);
		spMaskedSprite _masked;

	private:
		int klawisz;
		int num_of_players;

		int mainPlayerID;

		//Dane

		///////server udp////////
		struct sockaddr_in si_other;
		int serverSocket;
		int slen;
		///////server udp////////

		string message;
		float delta;
		spcPlayer _player;
		SOCKET RecieveSocket;
		SOCKET SendSocket;
		int przes;
		vector <spcPlayer> players;//wektor z graczami
		//
		SDL_Event event;
		HANDLE send_message;
		spcMenu menu;
		spcLevel level;


		//Metody
		void doUpdate(const UpdateState &us);	//Overload metody wywo³ywana 
		int _onSDLEvent(SDL_Event *event);
		bool connectToServer();							//£¹czenie z serwerem
		void _onPlatform(Event *event);
};



