#include "cCheckBox.h"

cCheckBox::cCheckBox(int x, int y,bool *w, string tlabel, bool editable, string class_name){
	//ustawienie wartosci poczatkowych
	checked = Color::White;
	unchecked = Color::DarkGray;
	wartosc = w;
	isEditable = editable;
	if(class_name.length()>0) setName(class_name);

	//wczytanie sprite'a do zaznaczenia
	spchecked = new Sprite;
	spchecked->setResAnim(Assets::gameResources.getResAnim("checkbox_checked",oxygine::ep_show_warning));
	spchecked->setName("spchecked");
	spchecked->setPosition(-2,-4);
	spchecked->setVisible(false);

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

	spchecked->attachTo(check);
};


//---Event nasluchiwania na klikniecie 
void cCheckBox::onClick(Event *ev){
	if(isEditable){
		if(*wartosc==true){
			check->setColor(unchecked);
			*wartosc = false;
			spchecked->setVisible(false);
		}
		else{
			check->setColor(checked);
			*wartosc=true;
			spchecked->setVisible(true);	
		}
	}
};

//---Zmienia mozliwosc edytowania checkboxa
void cCheckBox::onOff(bool flag){
	isEditable = flag;
};

//---Zmienia wartosc checkboxa na podana flage
void cCheckBox::setFlag(bool flag){
	*wartosc = flag;
};