#include "cButton.h"

cButton::cButton(int pX, int pY, string tekst, EventCallback c, string name, bool active){
	//inicjalizacja zmiennych skladowych
	x=pX;
	y=pY;
	cb = c;

	if(name.length()>0) setName(name);
	
	//dodanie wewnetrznych funkcji
	addBackground();
	addText(tekst);
	onOff(active);
};

//---Dodaje wysrodkowany tekst do przycisku
void cButton::addText(string t){
	text=cUI::createText(t);
	text->attachTo(background);
	text->setHAlign(TextStyle::HALIGN_LEFT);
	text->setPosition(30,background->getHeight()/2-5);
};

//---Dodaje tlo dla przycisku
void cButton::addBackground(){
	background = new Sprite;
	background->setResAnim(Assets::gameResources.getResAnim("button"));

    background->setPosition(x,y);		
    background->attachTo(this);
};	

//---Wlacza lub wylacza mozliwosc aktywacji przycisku		
void cButton::onOff(bool t){
	active = t;
	this->removeAllEventListeners();

	//dodanie obslugi zdarzen
	if(active){
		background->setAnimFrame(Assets::gameResources.getResAnim("button"),0);
		addEventListener(TouchEvent::CLICK, cb);
		addEventListener(TouchEvent::OVER, CLOSURE(this, &cButton::onMouseOver));
		addEventListener(TouchEvent::OUT, CLOSURE(this, &cButton::onMouseOut));
	}
	else
		background->setAnimFrame(Assets::gameResources.getResAnim("button"),2);
}; 

//---Obsluga zdarzenia po najechaniu myszka
void cButton::onMouseOver(Event *ev){
	background->setAnimFrame(Assets::gameResources.getResAnim("button"),1);
};

//---Obsluga zdarzenia po zjechaniu myszka
void cButton::onMouseOut(Event *ev){
	background->setAnimFrame(Assets::gameResources.getResAnim("button"),0);
};
