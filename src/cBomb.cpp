#include "cBomb.h"

cBomb::cBomb(int x, int y){
	pozx=x;
	pozy=y;

	bomb = new Sprite;
	bomb->setResAnim(Assets::gameResources.getResAnim("bomb"));

	bomb->setPosition(pozx-5,pozy-5);
	bomb->attachTo(this);
	bomb->setPriority(10);

	fire = new Sprite;
	fire->setResAnim(Assets::gameResources.getResAnim("bomb_fire"));

	range = 4;
};

//---Uruchamia TweenAlpha z wybuchem
void cBomb::destroy(){
	spTweenQueue tween = new TweenQueue();

	//efekt powiekszenia i zrobienia sie bialym
	tween->add(Sprite::TweenScale(1.1),50);
	tween->add(Sprite::TweenScale(1.3),50);
	tween->add(Sprite::TweenColor(Color::Red),50);
	tween->add(Sprite::TweenColor(Color::White),50);
	tween->add(TextActor::TweenAlpha(0),50);
	tween->add(TextActor::TweenAlpha(255),50);
	tween->add(TextActor::TweenAlpha(0),50);
	bomb->addTween(tween);

	//efekt ognia
	for(int i=-range; i<=range; i++){
		//poziom
		spTweenQueue fired = new TweenQueue();
		fired->add(TextActor::TweenAlpha(255),100);
		fired->add(Sprite::TweenScale(1.1),100);
		fired->add(Sprite::TweenScale(1.0),100);
		fired->add(Sprite::TweenScale(1.1),100);
		fired->add(TextActor::TweenAlpha(0),100);
		fired->setDetachActor(true);

		spSprite copy = fire->clone();
		copy->setPriority(0);
		copy->setPosition(pozx+i*64, pozy);
		copy->setAlpha(200);
		copy->attachTo(this);
		copy->addTween(fired);

		//pion
		spTweenQueue fired2 = new TweenQueue();
		fired2->add(TextActor::TweenAlpha(255),100);
		fired2->add(Sprite::TweenScale(1.1),100);
		fired2->add(Sprite::TweenScale(1.0),100);
		fired2->add(Sprite::TweenScale(1.1),100);
		fired2->add(TextActor::TweenAlpha(0),100);
		fired2->setDetachActor(true);

		spSprite copy2 = fire->clone();
		copy2->setPriority(0);
		copy2->setPosition(pozx, pozy+i*64);
		copy2->setAlpha(200);
		copy2->attachTo(this);
		copy2->addTween(fired2);
	}
	
};

 //---Ustawia zasieg razenia bomby po wybuchu
void cBomb::setRange(int range){
	this->range = range;
};