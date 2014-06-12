#pragma once
#include "cUI.h"
#include "InputText.h"
#include "Actor.h"
#include "Button.h"
#include "RenderState.h"
#include "oxygine-framework.h"

DECLARE_SMART(cCheckBox, spcCheckBox);

//---KLASA ODPOWIEDZIALNA ZA OBSLUGE POL TYPU TEXTFIELD
class cCheckBox: public Actor{
	public:	
		cCheckBox(int x, int y,bool *w, string tlabel="", bool editable=true, string class_name="");

		void onOff(bool flag); // - Zmienia mozliwosc edytowania checkboxa
		void setFlag(bool flag); // - Zmienia wartosc checkboxa na podana flage
	private:
		Color checked, unchecked; // - Kolory zanzaczenia
		spColorRectSprite ramka, check;
		spTextActor label;// - Labelka skojarzona z checkboxem
		bool *wartosc;
		bool isEditable;
		
		spSprite spchecked;

		void onClick(Event *ev);// - Event nasluchiwania na klikniecie 
};

