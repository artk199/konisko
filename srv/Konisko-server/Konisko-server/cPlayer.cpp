#include "cPlayer.h"
#include "cBomb.h"
#include "cLevel.h"

cPlayer::cPlayer(void)
{
	this->pos = std::pair<double,double>(160,96);
	this->direction = LEFT;
	this->velocity = 128;
	this->nick = "NICK";
	this->ilosc_bomb = 0;
	this->max_bomb = 1;
	for (int i=0;i<max_bomb;i++)
		bombs.push_back(NULL);
}


cPlayer::~cPlayer(void)
{
}

void cPlayer::setNick(string nick){
	this->nick = nick;
}

std::pair<double,double> cPlayer::getNextPos(double delta){
	return std::pair<double,double>(0,0);
}

void cPlayer::setPos(double x, double y){
	pos = std::pair<double,double>(x,y);
}
void cPlayer::move(double delta){
	double  newX = 0,
			newY = 0;
	switch (direction){
	case TOP:
		newX = this->pos.first;
		newY = this->pos.second - delta * this->velocity;
		break;
	case BOT:
		newX = this->pos.first;
		newY = this->pos.second + delta * this->velocity;
		break;
	case LEFT:
		newX = this->pos.first - delta * this->velocity;
		newY = this->pos.second;
		break;
	case RIGHT:
		newX = this->pos.first + delta * this->velocity;
		newY = this->pos.second;
		break;
	}

	if (map->canMove(newX,newY)){
		this->pos.first = newX;
		this->pos.second = newY;
	}else{
		direction = STAND;
	}
}

void cPlayer::update(double delta){
	this->move(delta);
	for(int i=0;i<max_bomb;i++){
		if(bombs[i] != NULL){
			bombs[i]->update(delta);
			if(bombs[i]->isDestroyed()){
				lvl->BOOM(bombs[i]->getPosition(),2);
				delete bombs[i];
				bombs[i] = NULL;
			}
		}
	}

}

void cPlayer::attachToMap(cMap* map){
	this->map = map;
}

void cPlayer::addBomb(){
	if(map == NULL){
		return;
	}
	cBomb* bomb = new cBomb();
	bomb->setPosition(this->map->getPositionOnMap(pos.first,pos.second));
	ilosc_bomb++;
	for(int i=0;i<max_bomb;i++){
		if(bombs[i] == NULL){
			bombs[i] = bomb;
			bomb->setId(i);
			return;
		}
	}
}

void cPlayer::changeDirection(E_DIRECTION direction){
	this->direction = direction;
}

void cPlayer::setConnection(connection* c1){
	this->c = c1;
}

connection * cPlayer::getConnection(){
	return c;
}

std::string cPlayer::serialize(){
	long double temp_id = this->id;
	string s =
		"object\tplayer\t"+std::to_string(temp_id)+/*tutaj jeszcze hashcode powinien byæ*/
		+"\nposition\t"+std::to_string((long double)pos.first) + "\t" + std::to_string((long double)pos.second)+
		"\ndirection\t"+std::to_string((long double)direction)+"\n";
	
	for(int i=0;i<max_bomb;i++){
		if(bombs[i] != NULL){
			s+=bombs[i]->serialize();
		}
	}

	return s;
}

