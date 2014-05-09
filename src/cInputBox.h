#pragma once
#include "cUI.h"
#include "InputText.h"
#include "Actor.h"
#include "Button.h"
#include "RenderState.h"
#include "cMap.h"
#include "cPlayer.h"
#include "oxygine-framework.h"
#include "Assets.h"

#include "cGame.h"

DECLARE_SMART(cInputBox, spcInputBox);

//---KLASA ODPOWIEDZIALNA ZA OBSLUGE POL TYPU TEXTFIELD
class cInputBox: public Actor{
	public:	
		cInputBox(int x, int y,string &t, string tlabel);
		~cInputBox(){InputText::stopAnyInput();}
		string getText();// - Pobiera aktualnie zapisany tekst na labelce

	private:
		Color noEdit, edited; // - Kolory tla bez edycji i w trakcie edytowania
		spInputText input;
		spTextActor text;// - Text wyswietlany w inputboxie
		ColorRectSprite *ramka;// - Tlo wokol inputboxa
		spTextActor label;// - Labelka skojarzona z inputboxem
		string *napis;


		void onComplete(Event *ev);// - Event po zaakceptowaniu wprowadzonych zmian
		void onClick(Event *ev);// - Event nasluchiwania na klikniecie 
};

