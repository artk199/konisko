#include "oxygine-framework.h"
#include "cUI.h"
#include "Assets.h"

using namespace oxygine;

//---Zwraca przycisk z okreslonym tekstem
spButton cUI::addButton(int x, int y, string text, EventCallback cb, string buttonStyle){
	spButton button = new Button();
	//pobranie grafiki ze stylow
	button->setResAnim(Assets::gameResources.getResAnim(buttonStyle));
	//ustawienie pozycji
	Vector2 pos;
	pos.x=x;
	pos.y=y;
	button->setPosition(pos);
	button->setInputChildrenEnabled(false);

	//funkcja obslugujaca przycisk
	button->addEventListener(TouchEvent::CLICK, cb);


	//przypisanie tekstu do przycisku
	spTextActor napis = cUI::createText(text, Color::White, "main");
	napis->attachTo(button);
	napis->setPosition(button->getSize()/2);

	return button;
};

//---Tworzy i zwraca obiekt typu tekst (nazwa czcionki z pliku XML)
spTextActor cUI::createText(string &txt, Color fcolor, string fontName){
	spTextActor text = new TextActor();

	TextStyle style;
	style.font = Assets::gameResources.getResFont(fontName)->getFont();
	style.color = fcolor;
	style.vAlign = TextStyle::VALIGN_MIDDLE;
	style.hAlign = TextStyle::HALIGN_CENTER;
	style.multiline = true;

	text->setStyle(style);
	text->setText(txt.c_str());

	return text;
};

//---Tworzy i zwraca efekt shake'ujacego sie tekstu, ktory nastepnie powieksza sie i znika
spTextActor cUI::addShakingText(int x, int y, string text, Color kolor){
	spTweenQueue tween = new TweenQueue();

	spTextActor nap= createText(text, kolor);
	nap->setX(x);
	nap->setY(y);

	int duration=20;
	//ilosc powtorzen efektow przesuwania
	for(int i=0; i<2; i++){
		tween->add(TextActor::TweenPosition(Vector2(x-2,y)),duration);
		tween->add(TextActor::TweenPosition(Vector2(x-4,y-2)),duration);
		tween->add(TextActor::TweenPosition(Vector2(x-3,y-2)),duration);
		tween->add(TextActor::TweenPosition(Vector2(x-6,y-4)),duration);

		tween->add(TextActor::TweenPosition(Vector2(x+2,y)),duration);
		tween->add(TextActor::TweenPosition(Vector2(x+4,y+2)),duration);
		tween->add(TextActor::TweenPosition(Vector2(x+3,y-2)),duration);
		tween->add(TextActor::TweenPosition(Vector2(x-6,y+4)),duration);
		tween->add(TextActor::TweenPosition(Vector2(x+4,y-4)),duration);
		tween->add(TextActor::TweenPosition(Vector2(x-2,y+2)),duration);
		tween->add(TextActor::TweenPosition(Vector2(x+2,y)),duration);
		tween->add(TextActor::TweenPosition(Vector2(x+2,y+2)),duration);
		tween->add(TextActor::TweenPosition(Vector2(x+2,y+2)),duration);

		tween->add(TextActor::TweenPosition(Vector2(x-4,y-4)),duration);
		tween->add(TextActor::TweenPosition(Vector2(x-2,y-2)),duration);
		tween->add(TextActor::TweenPosition(Vector2(x-2,y)),duration);
		tween->add(TextActor::TweenPosition(Vector2(x+2,y-2)),duration);
		tween->add(TextActor::TweenPosition(Vector2(x+2,y+2)),duration);
	}

	//efekt powiekszenia i rozmycia
	tween->add(TextActor::TweenScale(1.5),100);
	tween->add(TextActor::TweenAlpha(0),100);
	tween->setDetachActor(true);
	nap->addTween(tween);
		
	return nap;
};