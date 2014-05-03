#pragma once
#include "oxygine-framework.h"
#include "TextActor.h"

using namespace oxygine;

//---KLASA ODPOWIEDZIALNA ZA INTERFEJS U¯YTKOWNIKA
class cUI{
	public:
		//-v-Tworzy i zwraca przycisk (styl z XML) z okreslonym tekstem 
		static spButton addButton(int x, int y, string text, EventCallback cb, string buttonStyle="button"); 
		//-v-Tworzy i zwraca obiekt typu tekst (nazwa czcionki z pliku XML)
		static spTextActor createText(string &txt, Color fcolor=Color::Color(255,255,255,255), string fontName="main");	
};

