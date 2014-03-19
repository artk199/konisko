#pragma once
#include "Actor.h"
#include "Button.h"
#include "RenderState.h"

#include "oxygine-framework.h"
#include "cAssets.h"

using namespace oxygine;

class cGame: public Actor
{
	public:
		cGame();
		cAssets *assets;

		spTextActor _text;//testowa aplikacja
		void start();//testowa aplikacja


		static void Game_Initialize(); // - Funkcja ustawia wartosci poczatkowe dla klasy
		static void Game_Destroy(); // - Funkcja czysci pamiec po zakonczeniu zycia klasy
		static void Game_Update(); // - Funkcja aktualizujaca czynnosci klasy

	private:
		void displayClicked(Event *event);//testowa aplikacja
		void runSprite();//testowa aplikacja
};



