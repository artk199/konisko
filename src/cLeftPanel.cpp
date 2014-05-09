#include "cLeftPanel.h"
#include "cMenu.h"
#include "cLevel.h"
#include "cUI.h"
#include "cCheckBox.h"

cLeftPanel::cLeftPanel(spActor p,vector <spcPlayer> *player, spcMenu m){
	players = player;
	x=760;
	parent = p;
	setPriority(100);
	szer = 200;
	menu=m;
	wygaszaj=true;
	
	onMouseOut(NULL);
	//dodawanie dzieci panelu
	addBackground(background);
	addPlayersInfo();

	addBackground(topLayer,Color::White,1);
	topLayer->addEventListener(TouchEvent::OVER, CLOSURE(this, &cLeftPanel::onMouseOver));
	topLayer->addEventListener(7, CLOSURE(this, &cLeftPanel::onMouseOut));
	topLayer->setPriority(3);

	addButtons();
};

//---Ustawia wskaznik na klase odpowiadajaca za wyswietlanie menu
void cLeftPanel::setMenu(spcMenu m){menu=m;};

//---Zmienia pozycje panelu na ekranie		
void cLeftPanel::setXPosition(int newx){x=newx;};

//---Funkcja dodaje tlo o zadanym kolorze i przezroczystosci
void cLeftPanel::addBackground(spColorRectSprite &rect, Color kolor, int alpha){
	rect = new ColorRectSprite;
	rect->setHeight(Assets::windowSize.y);
	rect->setWidth(szer);
	rect->setX(x);
	rect->setY(0);
	rect->setColor(kolor);
	rect->setAlpha(alpha);
	rect->attachTo(this);
	rect->setPriority(0);
};

//---Dodaje informacje o graczach w panelu
void cLeftPanel::addPlayersInfo(){
	int ilosc = players->size();
	int przes=50;
	for(int i=0; i<ilosc; i++){
		przes+=i*50;

		spTextActor t = cUI::createText(players->at(i)->getName());
		t->setX(x+50);
		t->setY(przes);
		t->setHAlign(TextStyle::HALIGN_LEFT);
		t->setPriority(2);
		t->attachTo(this);

		spSprite s =players->at(i)->getIcon()->clone();
		s->setScale(0.5);
		s->setX(x+10);
		s->setY(przes-10);
		s->setPriority(2);
		s->attachTo(this);
	}
};

//---Dodaje przyciski dostepu w panelu
void cLeftPanel::addButtons(){
	EventCallback c1 = CLOSURE(menu.get(), &cMenu::menuMain);
	mainMenu=cUI::addButton(x+5,getRoot()->getHeight()-220, "Main Menu", c1);
	mainMenu->setPriority(4);
	mainMenu->attachTo(this);	

	spcCheckBox checkbox = new cCheckBox(x+5,getRoot()->getHeight()-140,&wygaszaj,"Shading");
	checkbox->setPriority(4);
	checkbox->attachTo(this);
}; 

//---Obsluga zdarzenia po najechaniu myszka
void cLeftPanel::onMouseOver(Event *ev){
	spTweenQueue tween = new TweenQueue();
	tween->add(TextActor::TweenAlpha(255),300);
	this->addTween(tween);
};

//---Obsluga zdarzenia po zjechaniu myszka
void cLeftPanel::onMouseOut(Event *ev){
	if(wygaszaj){
		spTweenQueue tween = new TweenQueue();
		tween->add(TextActor::TweenAlpha(20),300);
		this->addTween(tween);
	}
};