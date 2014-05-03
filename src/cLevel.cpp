#include "cLevel.h"
#include "cMenu.h"
#include "cNotify.h"


DECLARE_SMART(cLevel, spcLevel);

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
	menu->setVisible(false);
	setVisible(true);
	removeChildren();

	cGame::notifies->notify("test");
	map = new cMap();
	addChild(map);

	EventCallback c1 = CLOSURE(menu, &cMenu::menuMain);
	addChild(cUI::addButton(getRoot()->getWidth()-220,getRoot()->getHeight()-220, "Main Menu", c1));
};

//---Zapisanie wskaznika na klase wyswietlania menu na ekranie
void cLevel::setMenu(cMenu *m){menu=m;}