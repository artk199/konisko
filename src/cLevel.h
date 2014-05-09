#pragma once
#include "Actor.h"
#include "Button.h"
#include "RenderState.h"
#include "cMap.h"
#include "cPlayer.h"
#include "oxygine-framework.h"
#include "Assets.h"
#include "cUI.h"
#include "cMap.h"
#include "cLeftPanel.h"

class cMenu;
class cGame;
DECLARE_SMART(cGame, spcGame);
DECLARE_SMART(cMenu, spcMenu);
DECLARE_SMART(cLevel, spcLevel);

//---KLASA ZARZADZAJACA WYSWIETLANIEM ROZGRYWKI W GRZE
class cLevel: public Actor{
	public:
		cLevel();
		IVideoDriver *driver;
		spcMap map;

		void render(const RenderState &parentRS);

		void drawGame(Event *event);// - Zainicjalizowanie rysowania rozgrywki na ekranie
		void setMenu(spcMenu m);// - Zapisanie wskaznika na klase wyswietlania menu na ekranie
		void setGame(spcGame g);// - Zapisanie wskaznika na klase glowna programu
		void setPlayers(vector <spcPlayer> *p);// - Zapisanie wskaznika na wektor z graczami

	private:
		spcMenu menu;
		spcGame game;
		spcLeftPanel panel;
		vector <spcPlayer> *players;
};
