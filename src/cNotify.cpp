#include "oxygine-framework.h"
#include "cNotify.h"
#include "cAssets.h"
#include "cGame.h"

using namespace oxygine;

int cNotify::notifies[maxNotifies];
spActor cNotify::ui;
cNotify *cNotify::parent;

//---Wyswietla informacje w gornej czesci ekranu
void cNotify::notify(string txt, int time, Color tcolor){
	//ustalanie porzadku informacji
	size_t N = 0;
	for (size_t i = 0; i < maxNotifies; ++i){
		if (notifies[i]) continue;
		N = i;
		break;
	}

	notifies[N] += 1;

	//ustawienia paska tla
	spColorRectSprite sprite = new ColorRectSprite();
	sprite->setUserData((void*)N);
	sprite->setColor(tcolor);
	sprite->setSize((txt.length()+2)*11, 30);
	sprite->setAlpha(1);

	spTweenQueue tq = new TweenQueue;
	tq->add(Actor::TweenAlpha(255), 300, 1, false, 0, Tween::ease_inExpo);
	tq->add(Actor::TweenAlpha(0), 300, 1, false, 1200);
	tq->setDetachActor(true);
	tq->setDoneCallback(CLOSURE(parent, &cNotify::endNotify));

	sprite->addTween(tq);
	sprite->attachTo(ui);
	sprite->setPosition(getRoot()->getWidth()/2.0f - (txt.length()+2)*11/2, 30.0f + N * sprite->getHeight() * 1.1f);

	//ustawienia tekstu
	spTextActor text = createText(txt);
	text->attachTo(sprite);
	text->setPosition(sprite->getSize()/2);
};

//---Konczy wyswietlanie informacji na ekranie
void cNotify::endNotify(Event *ev){
	size_t N = size_t(ev->currentTarget->getUserData());
	notifies[N] -= 1;
};

//---Tworzy i zwraca obiekt typu tekst
spTextActor cNotify::createText(string txt, Color fcolor){
	spTextActor text = new TextActor();

	TextStyle style;
	style.font = cAssets::gameResources.getResFont("notify")->getFont();
	style.color = fcolor;
	style.vAlign = TextStyle::VALIGN_MIDDLE;
	style.hAlign = TextStyle::HALIGN_CENTER;
	style.multiline = true;

	text->setStyle(style);
	text->setText(txt.c_str());

	return text;
};
