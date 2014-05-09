#include "cLevel.h"
#include "cMenu.h"
#include "cNotify.h"


cLevel::cLevel():driver(0){
	setVisible(false);
	game=NULL;
	menu=NULL;
	map=NULL;
};

void cLevel::render(const RenderState &parentRS){
	parentRS.renderer->drawBatch();

	RenderState rs = parentRS;
	Renderer renderer = *parentRS.renderer;
	renderer.cleanup();
	renderer.setDriver(driver ? driver : IVideoDriver::instance);
	rs.renderer = &renderer;
	Actor::render(rs);
};

//---Zainicjalizowanie rysowania rozgrywki na ekranie
void cLevel::drawGame(Event *event){
	//proba polaczenia z serwerem
	try{
		game->tryConnectToServer();
	
		//proba zakonczyla sie powodzeniem
		menu->setVisible(false);
		setVisible(true);
		removeChildren();
	
		map = new cMap();
		addChild(map);

		panel = new cLeftPanel(this,players,menu);	
		panel->attachTo(this);
	}
	//wyswietlenie wyjatku na ekranie
	catch(int e){cGame::notifies->notify(Assets::errors[e].c_str());}	
};

//---Zapisanie wskaznika na klase wyswietlania menu na ekranie
void cLevel::setMenu(spcMenu m){menu=m;}

//---Zapisanie wskaznika na klase glowna programu
void cLevel::setGame(spcGame g){game=g;};

//---Zapisanie wskaznika na wektor z graczami
void cLevel::setPlayers(vector <spcPlayer> *p){players=p;};