#pragma once
#include "oxygine-framework.h"
#include "TextActor.h"
#include "cButton.h"

using namespace oxygine;

DECLARE_SMART(cButton, spcButton);

//---KLASA ODPOWIEDZIALNA ZA INTERFEJS U¯YTKOWNIKA
class cUI{
	public:
		//-v-Tworzy i zwraca przycisk (styl z XML) z okreslonym tekstem 
		static spcButton addButton(int x, int y, string text, EventCallback cb, string buttonName="", string buttonStyle="button"); 
		//-v-Tworzy i zwraca obiekt typu tekst (nazwa czcionki z pliku XML)
		static spTextActor createText(string &txt, Color fcolor=Color::Color(255,255,255,255), string fontName="main");	
		//-v-Tworzy i zwraca efekt shake'ujacego sie tekstu, ktory nastepnie powieksza sie i znika
		static spTextActor cUI::addShakingText(int x, int y, string text, Color kolor = Color::White);
};

