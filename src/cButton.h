#pragma once 
#include "oxygine-framework.h"
#include "cUI.h"
#include "Assets.h"
#include "TextActor.h"

DECLARE_SMART(cButton, spcButton);

class cButton:public Actor{
	spSprite background;
	spTextActor text;
	int x, y;
	bool active;
	EventCallback cb; 

	void addText(string t);// - Dodaje wysrodkowany tekst do przycisku
	void addBackground();// - Dodaje tlo dla przycisku
	public:
		cButton(int x, int y, string tekst, EventCallback c, string name="", bool active = true);
		void onOff(bool t); // - Wlacza lub wylacza mozliwosc aktywacji przycisku

		void onMouseOver(Event *ev);// - Obsluga zdarzenia po najechaniu myszka
		void onMouseOut(Event *ev);// - Obsluga zdarzenia po zjechaniu myszka
};