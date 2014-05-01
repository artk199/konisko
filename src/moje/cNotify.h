#pragma once
#include "oxygine-framework.h"
#include "cGame.h"
using namespace oxygine;


//---KLASA ZARZADZAJACA POWIADOMIENIAMI NA EKRANIE
class cNotify{
	public:
	    enum {maxNotifies = 6};// - Liczba informacji, ktora moze na raz znajdowac sie na ekranie
		int notifies[maxNotifies];
		spActor ui;

		void endNotify(Event *ev); // - Konczy wyswietlanie informacji na ekranie
		void notify(string text, int time = 400, Color tcolor=Color::Color(0,0,0,200)); // - Wyswietla informacje w gornej czesci ekranu
};

