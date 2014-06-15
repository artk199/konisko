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
	visible=true;
	direction = 5;
	velocity = 128;
	id=-1;

	sprite->setManageResAnim(true);

	//dodanie 10 pustych bomb
	for(int i=0; i<10; i++)		bombs.push_back(NULL);
};

void cPlayer::move(Vector2 delta){
	this->pos += delta;
	sprite->setPosition(pos.x,pos.y);
};

void cPlayer::setPosition(double x, double y){
	pos.x = x;
	pos.y = y;

	sprite->setPosition(pos.x-32,pos.y-32);
	//std::cout<<pos.x<<" "<<pos.y<<endl;
	poprzpoz = pos;
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
		}else
		if(sub == "bomb"){
			iss >> sub;
			int id = atoi(sub.c_str());
			iss >> sub;
			double x = atof(sub.c_str());
			iss >> sub;
			double y = atof(sub.c_str());
			iss >> sub;
			double time = atof(sub.c_str());
			this->addBomb(id,x*64,y*64);	
		}else
		if(sub == "direction"){
				iss >> sub;
				direction = atoi(sub.c_str());
		}
			
    } while (iss);
}

spSprite cPlayer::getIcon(){return sprite;};

void cPlayer::init(){
	visible = true;
	sprite->addTween(TweenAnim(Assets::gameResources.getResAnim("hero"+to_string(long double(id)),oxygine::ep_show_warning)),500,-1);
}


//---Aktualizuje bomby i wybucha te, na ktore przyszedl juz czas, by oposcic ten ziemski padol
void cPlayer::updateBombs(int dt){
	for(int i=0; i<10; i++)
		if(bombs[i]!=NULL){
			// jezeli zostal przekroczony czas, bomba wybucha
			if(bombs[i]->updateDestroyTime(dt)){
			}
		}
		//return;
	//printf("Direction: %d\n",direction);
	double delta = dt;
	delta /= 1000; 
	switch (direction){
	case 1:
		this->pos.y -= delta * this->velocity;
		break;
	case 2:
		this->pos.y += delta * this->velocity;
		break;
	case 3:
		this->pos.x -= delta * this->velocity;
		break;
	case 4:
		this->pos.x += delta * this->velocity;
		break;
	}
	sprite->setPosition(pos.x-32,pos.y-32);
};

//--Dodaje nowa bombe dla gracza pod jej adresem ID
void cPlayer::addBomb(int id,int x, int y, int range, int destroying_time){
	if(bombs[id] == NULL || bombs[id]->isDestroyed()){
		bombs[id] = new cBomb(x,y);
		bombs[id]->attachTo(this);
		bombs[id]->setDestroyTime(destroying_time);
		bombs[id]->setRange(2);
	}
};