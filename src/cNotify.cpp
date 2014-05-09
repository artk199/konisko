#include "oxygine-framework.h"
#include "cNotify.h"
#include "Assets.h"
#include "cGame.h"
#include "cUI.h"


using namespace oxygine;


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
	int szer=(txt.length()+2)*11;
	if(szer>500) szer=500;
	sprite->setSize(szer, 30);
	sprite->setAlpha(1);

	spTweenQueue tq = new TweenQueue;
	tq->add(Actor::TweenAlpha(255), 300, 1, false, 0, Tween::ease_inExpo);
	tq->add(Actor::TweenAlpha(0), 300, 1, false, 1200);
	tq->setDetachActor(true);
	tq->setDoneCallback(CLOSURE(this, &cNotify::endNotify));
	ui->setPriority(1000);
	sprite->addTween(tq);
	sprite->attachTo(ui);
	sprite->setPosition(getRoot()->getWidth()/2.0f - szer/2, 30.0f + N * sprite->getHeight() * 1.1f);

	//ustawienia tekstu
	spTextActor text = cUI::createText(txt, Color::White, "notify");
	text->attachTo(sprite);
	text->setPosition(sprite->getSize()/2);
};

//---Konczy wyswietlanie informacji na ekranie
void cNotify::endNotify(Event *ev){
	size_t N = size_t(ev->currentTarget->getUserData());
	notifies[N] -= 1;
};

