#pragma once
#include "Actor.h"
#include "Button.h"
#include "RenderState.h"

#include "oxygine-framework.h"
#include "cAssets.h"

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

		static void Game_Initialize(); // - Funkcja ustawia wartosci poczatkowe dla klasy
		static void Game_Destroy(); // - Funkcja czysci pamiec po zakonczeniu zycia klasy
		static void Game_Update(); // - Funkcja aktualizujaca czynnosci klasy

		cNotify *notifies;
	
	private:
		Content *content;
		
		void start();//testowa aplikacja
		void displayClicked(Event *event);//testowa aplikacja
		void runSprite();//testowa aplikacja
};



