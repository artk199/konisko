#include "cInputBox.h"

cInputBox::cInputBox( int x, int y, string &t, string tlabel){
	noEdit = Color::Chocolate;//RGB(48,68,72);
	edited = Color::DarkViolet;//RGB(72,48,48);

	napis = &t;

	//tworzenie labelki
	label = cUI::createText(tlabel);
	label->setHAlign(TextStyle::HALIGN_LEFT);
	label->setX(x);
	label->setY(y+10);
	label->attachTo(this);

	if(tlabel.length()>0) x+=200;

	//tworzenie ramki
	ramka = new ColorRectSprite();
	ramka->setX(x);
	ramka->setY(y);
	ramka->setWidth(200);
	ramka->setHeight(30);
	ramka->setColor(noEdit);
	ramka->attachTo(this);

	//tworzenie tekstu
	text = cUI::createText(t);
	text->setHAlign(TextStyle::HALIGN_LEFT);
	text->setX(x+5);
	text->setY(y+10);
	text->attachTo(this);

	//dodanie eventow
	ramka->addEventListener(TouchEvent::CLICK, CLOSURE(this, &cInputBox::onClick));
	text->addEventListener(TouchEvent::CLICK, CLOSURE(this, &cInputBox::onClick));
	input = new InputText;
	input->addEventListener(Event::COMPLETE, CLOSURE(this, &cInputBox::onComplete));
};

cInputBox::~cInputBox(){
	InputText::stopAnyInput();
	string pom=text->getText();
	*napis = pom;
};

//---Event po zaakceptowaniu wprowadzonych zmian
void cInputBox::onComplete(Event *ev){
	ramka->setColor(noEdit);
	string pom=text->getText();
	*napis = pom;
	InputText::stopAnyInput();
};

//---Event nasluchiwania na klikniecie 
void cInputBox::onClick(Event *ev){
	ramka->setColor(noEdit);
	input->start(text);
	ramka->setColor(edited);
};

//---Pobiera aktualnie zapisany tekst na labelce
string cInputBox::getText(){return text->getText();};
