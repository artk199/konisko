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
class cMenu;
class cLevel;
using namespace oxygine;

class cNotify;


typedef void (*handler) (Event);


class cGame: public Actor
{
	public:
		cGame();

		void init();		// - Funkcja ustawia wartosci poczatkowe dla klasy
		void destroy();		// - Funkcja czysci pamiec po zakonczeniu zycia klasy

		static cNotify *notifies;
		//Thread odpowiedzialny za wysy³anie operacji
		DWORD sender(void);
		DWORD reciever(void);

	private:
		//Dane
		float delta;
		cMap* _map;
		cPlayer* _player;
		SOCKET ConnectSocket;
		int przes;
		//
		SDL_Event event;
		HANDLE send_message;
		cMenu *menu;
		cLevel *level;

		//Metody
		void doUpdate(const UpdateState &us);	//Overload metody wywo³ywana 
		int _onSDLEvent(SDL_Event *event);
		void connectToServer();							//£¹czenie z serwerem
		void disconnect();						//Roz³¹czenie z serwerem
		void _onPlatform(Event *event);
		void start();							//testowa aplikacja

		void runSprite();						//testowa aplikacja
	    void displayClicked(Event *event);		//testowa aplikacja
};



