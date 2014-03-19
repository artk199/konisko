#pragma once
#include "oxygine-framework.h"
#include "cGame.h"
using namespace oxygine;

static class cNotify{
	public:
		static enum {maxNotifies = 6};// - Liczba informacji, ktora moze na raz znajdowac sie na ekranie
		static int notifies[maxNotifies];
		static spActor ui;
		static cNotify *parent;

		void endNotify(Event *ev); // - Konczy wyswietlanie informacji na ekranie
		static spTextActor createText(string txt, Color fcolor=Color::Color(255,255,255,255)); // - Tworzy i zwraca obiekt typu tekst
		static void notify(string text, int time = 400, Color tcolor=Color::Color(0,0,0,200)); // - Wyswietla informacje w gornej czesci ekranu
};

