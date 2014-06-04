#include "cMenu.h"
#include "cLevel.h"
#include "InputText.h"
#include "cInputBox.h"
#include "cNotify.h"

void cMenu::render(const RenderState &parentRS){
	parentRS.renderer->drawBatch();

	RenderState rs = parentRS;
	Renderer renderer = *parentRS.renderer;
	renderer.cleanup();
	renderer.setDriver(driver ? driver : IVideoDriver::instance);
	rs.renderer = &renderer;
	Actor::render(rs);
};

//---Pokazuje na ekranie opcje menu glownego
void cMenu::menuMain(Event *event){
	if(level!=NULL){
		level->setVisible(false);
		//level->map->clear();
	}
	
	game->disconnect();

	setVisible(true);
	this->removeChildren(); //czyszczenie wszystkich dzieci

	EventCallback c1 = CLOSURE(this, &cMenu::menuMultiplayer);
	addChild(cUI::addButton(getRoot()->getWidth()-220,getRoot()->getHeight()-220, "Multiplayer", c1, "bt_multi"));
	EventCallback c2 = CLOSURE(this, &cMenu::menuOptions);
	addChild(cUI::addButton(getRoot()->getWidth()-220,getRoot()->getHeight()-150, "Options", c2, "bt_options"));
};

//---Pokazuje na ekranie opcje menu zwiazanego z ustawieniami
void cMenu::menuOptions(Event *event){
	setVisible(true);
	this->removeChildren(); //czyszczenie wszystkich dzieci

	addChild(new cInputBox(50,50,Assets::userNick,"Nick:"));
	addChild(new cInputBox(50,100,Assets::serverName,"Nazwa serwera:"));
	
	EventCallback c = CLOSURE(this, &cMenu::menuMain);
	addChild(cUI::addButton(getRoot()->getWidth()-220,getRoot()->getHeight()-150, "back", c,"bt_back"));
};

//---Pokazuje na ekranie opcje gry w sieci
void cMenu::menuMultiplayer(Event *event){
	setVisible(true);
	this->removeChildren(); //czyszczenie wszystkich dzieci
	//proba polaczenia z serwerem
	try{
		game->tryConnectToServer();
	
		EventCallback c1 = CLOSURE(level.get(), &cLevel::drawGame);
		addChild(cUI::addButton(getRoot()->getWidth()-220,getRoot()->getHeight()-220, "Start level", c1, "bt_start"));
		EventCallback c2 = CLOSURE(this, &cMenu::menuMain);
		addChild(cUI::addButton(getRoot()->getWidth()-220,getRoot()->getHeight()-150, "back", c2, "bt_back"));
	}
	//wyswietlenie wyjatku na ekranie
	catch(int e){cGame::notifies->notify(Assets::errors[e].c_str());}	

	
};

//---Zapisuje wskaznik na klase zarzadzajaca wyswietlaniem gry
void cMenu::setLevel(spcLevel g){level=g;};