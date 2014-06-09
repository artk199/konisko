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
	if(level!=NULL)
		level->setVisible(false);
	
	game->disconnect();

	setVisible(true);
	this->removeChildren(); //czyszczenie wszystkich dzieci

	EventCallback c1 = CLOSURE(this, &cMenu::menuMultiplayer);
	addChild(cUI::addButton(getRoot()->getWidth()-220,getRoot()->getHeight()-220, "Multiplayer", c1));
	EventCallback c2 = CLOSURE(this, &cMenu::menuOptions);
	addChild(cUI::addButton(getRoot()->getWidth()-220,getRoot()->getHeight()-150, "Options", c2));
};

//---Pokazuje na ekranie opcje menu zwiazanego z ustawieniami
void cMenu::menuOptions(Event *event){
	setVisible(true);
	this->removeChildren(); //czyszczenie wszystkich dzieci

	addChild(new cInputBox(50,50,Assets::userNick,"Nick:"));
	addChild(new cInputBox(50,100,Assets::serverName,"Nazwa serwera:"));

	EventCallback c = CLOSURE(this, &cMenu::menuMain);
	addChild(cUI::addButton(getRoot()->getWidth()-220,getRoot()->getHeight()-150, "back", c));
};

//---Pokazuje na ekranie opcje gry w sieci
void cMenu::menuMultiplayer(Event *event){

	try{
		setVisible(true);
		this->removeChildren(); //czyszczenie wszystkich dzieci


		EventCallback c1 = CLOSURE(level.get(), &cLevel::drawGame);
		addChild(cUI::addButton(getRoot()->getWidth()-220,getRoot()->getHeight()-220, "Start level", c1, "bt_start", false));
		EventCallback c2 = CLOSURE(this, &cMenu::menuMain);
		addChild(cUI::addButton(getRoot()->getWidth()-220,getRoot()->getHeight()-150, "back", c2));

		game->tryConnectToServer();
	}
	catch(int e){
		game->notifies->notify(Assets::errors[e]);
		menuMain(NULL);
	}
};

//---Zapisuje wskaznik na klase zarzadzajaca wyswietlaniem gry
void cMenu::setLevel(spcLevel g){level=g;};