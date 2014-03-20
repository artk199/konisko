#pragma once
#include "Actor.h"
#include "Button.h"
#include "RenderState.h"

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
		
		//Metody
		void doUpdate(const UpdateState &us);	//Overload metody wywo³ywana 


		void start();							//testowa aplikacja
		void displayClicked(Event *event);		//testowa aplikacja
		void runSprite();						//testowa aplikacja
};



