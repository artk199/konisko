#pragma once
#include "Actor.h"
#include "oxygine-framework.h"
#include "Assets.h"
#include "cUI.h"
#include "cPlayer.h"
#include "cCheckBox.h"

class cMenu;
class cLevel;

DECLARE_SMART(cLeftPanel, spcLeftPanel);
DECLARE_SMART(cMenu, spcMenu);
//---KLASA LEWEGO PANELU PODCZAS GRY
class cLeftPanel: public Actor{
	public:
		cLeftPanel(spActor p,vector <spcPlayer> *player, spcMenu m);

		void setXPosition(int newx); // - Zmienia pozycje panelu na ekranie
		void setMenu(spcMenu m);// - Ustawia wskaznik na klase odpowiadajaca za wyswietlanie menu
	
	private:
		int x;
		int szer;
		bool wygaszaj;

		vector <spcPlayer> *players;
		spActor parent;
		spColorRectSprite background, topLayer;
		spButton mainMenu;
		spcCheckBox checkbox;
		spcMenu menu;

		void addBackground(spColorRectSprite &rect, Color kolor=Color::Black, int alpha=200); // - Funkcja dodaje tlo o zadanym kolorze i przezroczystosci
		void addPlayersInfo(); // - Dodaje informacje o graczach w panelu
		void addButtons(); // - Dodaje przyciski dostepu w panelu

		void onMouseOver(Event *ev);// - Obsluga zdarzenia po najechaniu myszka
		void onMouseOut(Event *ev);// - Obsluga zdarzenia po zjechaniu myszka
};