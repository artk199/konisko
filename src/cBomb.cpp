#include "cBomb.h"

cBomb::cBomb(int x, int y, float time){
	startTime = time;
	pozx=x;
	pozy=y;

	bomb = new Sprite;
	bomb->setResAnim(Assets::gameResources.getResAnim("bomb"));

	bomb->setPosition(pozx-5,pozy-5);
	bomb->setPriority(10);
	bomb->attachTo(this);

	fire = new Sprite;
	fire->setResAnim(Assets::gameResources.getResAnim("bomb_fire"));

	range = 1;

	startTime=0;
	destroyTime = 3000;

	destroyed = false;
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
	tween->setDetachActor(true);

	spSprite bclone = bomb->clone();
	bclone->attachTo(this);
	bclone->addTween(tween);

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

 //---Zwraca czas rozstawienia bomby
float cBomb::getTime(){
	return startTime;
};

 //--- Ustawia czas, po jakim bomba wybucha
void cBomb::setDestroyTime(float t){
	destroyTime=t;
};

//---Zwraca czas, po jakim bomba wybucha
float cBomb::getDestroyTime(){
	return destroyTime;
}; 

//---Odejmuje od czasu wybuchu dt i wybucha po uplywie czasu
bool cBomb::updateDestroyTime(int dt){
	if(destroyed) return true;
	destroyTime -= dt;

	if(destroyTime <= 0) {
		destroy();
		destroyed = true;
		return true;
	}
	return false;
};
