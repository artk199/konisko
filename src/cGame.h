#pragma once
#include "Actor.h"
#include "Button.h"
#include "RenderState.h"

#include "oxygine-framework.h"
#include "cAssets.h"

using namespace oxygine;


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



class cGame: public Actor
{
	public:
		cGame();

		static void Game_Initialize(); // - Funkcja ustawia wartosci poczatkowe dla klasy
		static void Game_Destroy(); // - Funkcja czysci pamiec po zakonczeniu zycia klasy
		static void Game_Update(); // - Funkcja aktualizujaca czynnosci klasy

		spTextActor _text;//testowa aplikacja
		void start();//testowa aplikacja

	private:
		Content *content;
		
		
		void displayClicked(Event *event);//testowa aplikacja
		void runSprite();//testowa aplikacja
};



