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


//---KLASA ZARZADZAJACA WSZYSTKIMI MENU W GRZE
class cMenu: public Actor{
	public:
		cMenu():driver(0){game=NULL;menuMain(NULL);}
		IVideoDriver *driver;

		void render(const RenderState &parentRS);

		void menuMain(Event *event);// - Pokazuje na ekranie opcje menu glownego
		void menuOptions(Event *event);// - Pokazuje na ekranie opcje menu zwiazanego z ustawieniami
		void menuMultiplayer(Event *event);// - Pokazuje na ekranie opcje gry w sieci
		void setGame(cLevel *g);// - Zapisuje wskaznik na klase zarzadzajaca wyswietlaniem gry

	private:
		cLevel *game;
};
