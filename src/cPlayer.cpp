#include "cPlayer.h"
#include "Assets.h"
#include <sstream>
#include <iostream>


cPlayer::cPlayer(){
	pos.x=0;
	pos.y=0;
	nick="New player";
	sprite = new Sprite;
	sprite->attachTo(this);
	sprite->setPosition(pos.x,pos.y);
	ready=false;
	visible=false;
	id=-1;
};

void cPlayer::move(Vector2 delta){
	this->pos += delta;
	sprite->setPosition(pos.x,pos.y);
};
void cPlayer::setPosition(double x, double y){
	pos.x = x;
	pos.y = y;
	sprite->setPosition(pos.x-32,pos.y-32);
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

void cPlayer::init(){
	visible = true;
	sprite->addTween(TweenAnim(Assets::gameResources.getResAnim("hero"+to_string(long double(id)),oxygine::ep_show_warning)),500,-1);
}