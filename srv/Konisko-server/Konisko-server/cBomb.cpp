#include "cBomb.h"


cBomb::cBomb(void)
{
	this->time = 2.9;
	this->power = 2;

}


cBomb::~cBomb(void)
{
}

bool cBomb::isDestroyed(){
	return time <= 0;
}

void cBomb::update(double delta){
	this->time -= delta;
}

void cBomb::setPosition(int x, int y){
	pos.first = x;
	pos.second = y;
}

void cBomb::setPosition(std::pair<int,int> newPos){
	pos = newPos;
}

std::pair<int,int> cBomb::getPosition(){
	return this->pos;
}
string cBomb::serialize(){
	string r = "";
	r="bomb\t"+std::to_string((long double)id)+"\t"+std::to_string((long double)pos.first)+"\t"+std::to_string((long double)pos.second)+"\n";
	return r;
}
