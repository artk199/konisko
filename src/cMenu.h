#pragma once
#include "Actor.h"
#include "Button.h"
#include "RenderState.h"
#include "cMap.h"
#include "cPlayer.h"
#include "oxygine-framework.h"
#include "Assets.h"
#include "cUI.h"
#include "cGame.h"

class cLevel;
using namespace std;

DECLARE_SMART(cLevel, spcLevel);
DECLARE_SMART(cMenu, spcMenu);
DECLARE_SMART(cGame, spcGame);

//---KLASA ZARZADZAJACA WSZYSTKIMI MENU W GRZE
class cMenu: public Actor{
	public:
		cMenu(spcGame g, string name="");
		IVideoDriver *driver;

		void render(const RenderState &parentRS);

		void menuMain(Event *event);// - Pokazuje na ekranie opcje menu glownego
		void menuOptions(Event *event);// - Pokazuje na ekranie opcje menu zwiazanego z ustawieniami
		void menuMultiplayer(Event *event);// - Pokazuje na ekranie opcje gry w sieci
		void setLevel(spcLevel g);// - Zapisuje wskaznik na klase zarzadzajaca wyswietlaniem gry
		
	private:
		spcLevel level;
		spcGame game;
		spSprite background;

		void addBackground(int x=0, int y=0, int width = 0, int height=0);// - Dodaje obrazek dla do menu z opcja ustawienia przezroczystego contentu
};
