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
#include "cMap.h"

class cMenu;

//---KLASA ZARZADZAJACA WYSWIETLANIEM ROZGRYWKI W GRZE
class cLevel: public Actor{
	public:
		cLevel():driver(0){setVisible(false);}
		IVideoDriver *driver;
		cMap *map;

		void render(const RenderState &parentRS);

		void drawGame(Event *event);// - Zainicjalizowanie rysowania rozgrywki na ekranie
		void setMenu(cMenu *m);// - Zapisanie wskaznika na klase wyswietlania menu na ekranie
	private:
		cMenu *menu;
		
};
