#include "cPlayer.h"
#include "Assets.h"


cPlayer::cPlayer(Vector2 pos){
	this->pos = pos;
};

void cPlayer::_init(){
	sprite = new Sprite;
	sprite->addTween(TweenAnim(Assets::gameResources.getResAnim("player")),500,-1);
	sprite->setPosition(pos.x,pos.y);
	sprite->attachTo(_view);
};

void cPlayer::move(Vector2 delta){
	this->pos += delta;
	sprite->setPosition(pos.x,pos.y);
};


string cPlayer::getName(){return name;};
void cPlayer::setName(string n){name=n;};

spSprite cPlayer::getIcon(){return sprite;};