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

using namespace oxygine;

class cNotify;

class Content: public Actor{
public:
	Content():driver(0){}
	IVideoDriver *driver;

	void render(const RenderState &parentRS){
		parentRS.renderer->drawBatch();

		RenderState rs = parentRS;
		Renderer renderer = *parentRS.renderer;
		renderer.cleanup();
		renderer.setDriver(driver ? driver : IVideoDriver::instance);
		rs.renderer = &renderer;
		Actor::render(rs);
	}
};

typedef void (*handler) (Event);


class cGame: public Actor
{
	public:
		cGame();

		void init();		// - Funkcja ustawia wartosci poczatkowe dla klasy
		void destroy();		// - Funkcja czysci pamiec po zakonczeniu zycia klasy

		cNotify *notifies;
		//Thread odpowiedzialny za wysy³anie operacji
		DWORD sender(void);
		DWORD reciever(void);
	private:
		//Dane
		Content *content;
		float delta;
		cMap* _map;
		cPlayer* _player;
		SOCKET ConnectSocket;
		int przes;
		//
		SDL_Event event;
		HANDLE send_message;

		//Metody
		void doUpdate(const UpdateState &us);	//Overload metody wywo³ywana 
		int _onSDLEvent(SDL_Event *event);
		void connectToServer();							//£¹czenie z serwerem
		void disconnect();						//Roz³¹czenie z serwerem
		void _onPlatform(Event *event);
		void start();							//testowa aplikacja
		void displayClicked(Event *event);		//testowa aplikacja
		void runSprite();						//testowa aplikacja
};



