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
		cCheckBox(int x, int y,bool *w, string tlabel);

	private:
		Color checked, unchecked; // - Kolory zanzaczenia
		spColorRectSprite ramka, check;
		spTextActor label;// - Labelka skojarzona z checkboxem
		bool *wartosc;

		void onClick(Event *ev);// - Event nasluchiwania na klikniecie 
};
