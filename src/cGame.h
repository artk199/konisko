#pragma once
#include "Actor.h"
#include "Button.h"
#include "RenderState.h"
#include "cMap.h"
#include "cPlayer.h"
#include "oxygine-framework.h"
#include "Assets.h"

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
	
	private:
		//Dane
		Content *content;
		float delta;
		cMap* _map;
		cPlayer* _player;
		
		//
		SDL_Event event;

		//Metody
		void doUpdate(const UpdateState &us);	//Overload metody wywoływana 
		int _onSDLEvent(SDL_Event *event);
		void _onPlatform(Event *event);
		void start();							//testowa aplikacja
		void displayClicked(Event *event);		//testowa aplikacja
		void runSprite();						//testowa aplikacja
};



