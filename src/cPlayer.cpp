#include "cPlayer.h"
#include "Assets.h"
#include <sstream>
#include <iostream>


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
void cPlayer::setPosition(double x, double y){
	pos.x = x;
	pos.y = y;
	sprite->setPosition(pos.x,pos.y);
}
void cPlayer::deserialize(string s){
    istringstream iss(s);
    do
    {
        string sub;
        iss >> sub;
		if(sub == "position"){
			iss >> sub;
			double x = atof(sub.c_str());
			iss >> sub;
			double y = atof(sub.c_str());
			this->setPosition(x,y);
		}
			
    } while (iss);
}
spSprite cPlayer::getIcon(){return sprite;};