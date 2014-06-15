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
	wygaszaj=false;
	isAlpha=true;

	printf("%d - %d \n", &wygaszaj, wygaszaj);

	onMouseOut(NULL);
	//dodawanie dzieci panelu
	addBackground(background);
	addPlayersInfo();

	this->addEventListener(TouchEvent::MOVE, CLOSURE(this, &cLeftPanel::onMouseMove));
	this->addEventListener(7, CLOSURE(this, &cLeftPanel::onMouseOut));
	addButtons();

	setName("cLeftPanel");
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
	int ktory = 0;
	for(int i=0; i<ilosc; i++){
		int przes=ktory*50+50;

		if(players->at(i)->getID()>=0){
			spTextActor t = cUI::createText(players->at(i)->getNick());
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

			ktory++;
		}
	}
};

//---Dodaje przyciski dostepu w panelu
void cLeftPanel::addButtons(){
	EventCallback c1 = CLOSURE(menu.get(), &cMenu::menuMain);
	mainMenu=cUI::addButton(5,background->getHeight()-220, "Main Menu", c1);
	mainMenu->setPriority(4);
	mainMenu->attachTo(background);	

	spcCheckBox checkbox = new cCheckBox(5,background->getHeight()-140,&wygaszaj,"Stop shading");
	checkbox->setPriority(4);
	checkbox->attachTo(background);
}; 

//---Obsluga zdarzenia po najechaniu myszka
void cLeftPanel::onMouseOver(Event *ev){
	spTweenQueue tween = new TweenQueue();
	tween->add(TextActor::TweenAlpha(255),300);
	this->addTween(tween);
	isAlpha = false;
};

//---Obsluga zdarzenia po zjechaniu myszka
void cLeftPanel::onMouseOut(Event *ev){
	if(!wygaszaj){
		spTweenQueue tween = new TweenQueue();
		tween->add(TextActor::TweenAlpha(20),300);
		this->addTween(tween);
		isAlpha=true;
		
	}

};

//---Obsluga poruszania myszka
void cLeftPanel::onMouseMove(Event *ev){
	oxygine::spEventDispatcher pos = ev->currentTarget;

	if(isAlpha){
		if(pos->getName().c_str() == this->getName())
			onMouseOver(ev);
		else
			onMouseOut(ev);
	}
};