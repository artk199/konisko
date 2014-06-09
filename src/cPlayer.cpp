#include "cPlayer.h"
#include "Assets.h"


cPlayer::cPlayer(){
	pos.x=100;
	pos.y=100;
	nick="New player";
	sprite = new Sprite;
	sprite->addTween(TweenAnim(Assets::gameResources.getResAnim("player")),500,-1);
	sprite->setPosition(pos.x,pos.y);
	sprite->attachTo(this);
};

void cPlayer::move(Vector2 delta){
	this->pos += delta;
	sprite->setPosition(pos.x,pos.y);
};

spSprite cPlayer::getIcon(){return sprite;};