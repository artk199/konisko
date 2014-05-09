#include "cCheckBox.h"

cCheckBox::cCheckBox( int x, int y, bool *w, string tlabel){
	checked = Color::White;
	unchecked = Color::DarkGray;
	wartosc = w;

	//tworzenie labelki
	label = cUI::createText(tlabel);
	label->setHAlign(TextStyle::HALIGN_LEFT);
	label->setX(x+25);
	label->setY(y+5);
	label->attachTo(this);

	//tworzenie ramki
	ramka = new ColorRectSprite();
	ramka->setX(x);
	ramka->setY(y);
	ramka->setWidth(20);
	ramka->setHeight(20);
	ramka->setColor(Color::Black);
	ramka->attachTo(this);
	
	//tworzenie srodka
	Color k=unchecked;
	if(*wartosc==true) k=checked;
	check = new ColorRectSprite();
	check->setX(x+2);
	check->setY(y+2);
	check->setHeight(16);
	check->setWidth(16);
	check->setColor(k);
	check->attachTo(this);
	check->addEventListener(TouchEvent::CLICK, CLOSURE(this, &cCheckBox::onClick));
};


//---Event nasluchiwania na klikniecie 
void cCheckBox::onClick(Event *ev){
	if(*wartosc==true){
		check->setColor(unchecked);
		*wartosc = false;
	}
	else{
		check->setColor(checked);
		*wartosc=true;
	}
};
