#include "cMenu.h"
#include "cLevel.h"
#include "InputText.h"
#include "cInputBox.h"
#include "cNotify.h"

cMenu::cMenu(spcGame g, string name):driver(0){
	game=g;
	level=NULL;
	
	if(name.length()>0) setName(name);

	background = new Sprite;
	background->setResAnim(Assets::gameResources.getResAnim("background",oxygine::ep_show_warning));

	menuMain(NULL);
}
	
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

	addBackground();

	EventCallback c1 = CLOSURE(this, &cMenu::menuMultiplayer);
	addChild(cUI::addButton(getRoot()->getWidth()-220,getRoot()->getHeight()-220, "Multiplayer", c1));
	EventCallback c2 = CLOSURE(this, &cMenu::menuOptions);
	addChild(cUI::addButton(getRoot()->getWidth()-220,getRoot()->getHeight()-150, "Options", c2));
};

//---Pokazuje na ekranie opcje menu zwiazanego z ustawieniami
void cMenu::menuOptions(Event *event){
	setVisible(true);
	this->removeChildren(); //czyszczenie wszystkich dzieci

	addBackground(20,20,700,600); 

	//dodanie tytulu
	spTextActor napis = cUI::createText("Options",Color::Khaki);
	napis->attachTo(this);
	napis->setPosition(50,50);
	napis->setHAlign(TextStyle::HALIGN_LEFT);

	addChild(new cInputBox(100,100,Assets::userNick,"Nick:"));
	addChild(new cInputBox(100,150,Assets::serverName,"Nazwa serwera:"));
	addChild(new cInputBox(100,200,Assets::serverPort,"Port serwera:"));

	EventCallback c = CLOSURE(this, &cMenu::menuMain);
	addChild(cUI::addButton(getRoot()->getWidth()-220,getRoot()->getHeight()-150, "back", c));
};

//---Pokazuje na ekranie opcje gry w sieci
void cMenu::menuMultiplayer(Event *event){

	try{
		setVisible(true);
		this->removeChildren(); //czyszczenie wszystkich dzieci

		addBackground(20,20,700,600);

		//dodanie tytulu
		spTextActor napis = cUI::createText("Waiting for other players...",Color::Khaki);
		napis->attachTo(this);
		napis->setPosition(50,50);
		napis->setHAlign(TextStyle::HALIGN_LEFT);

		EventCallback c1 = CLOSURE(level.get(), &cLevel::drawGame);
		addChild(cUI::addButton(getRoot()->getWidth()-220,getRoot()->getHeight()-220, "Start level", c1, "bt_start", true));
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


//---Dodaje obrazek dla do menu z opcja ustawienia przezroczystego contentu
void cMenu::addBackground(int x, int y, int width, int height){
	background->setPosition(0,0);

	float scale = Assets::windowSize.x/background->getWidth();
	background->setScale(scale);
	background->attachTo(this);

	//podano obydwa rozmiary
	if(width && height){
		spColorRectSprite bg = new ColorRectSprite();
		bg->setPosition(x,y);
		bg->setWidth(width);
		bg->setHeight(height);
		bg->setColor(Color::Black);
		bg->attachTo(this);
		bg->setAlpha(220);
	}
};
