#include "cPlayer.h"


cPlayer::cPlayer(void)
{
	this->pos = std::pair<double,double>(160,96);
	this->direction = LEFT;
	this->velocity = 128;
	this->nick = "NICK";
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
	}
}

void cPlayer::update(double delta){
	this->move(delta);
}

void cPlayer::attachToMap(cMap* map){
	this->map = map;
}

void cPlayer::changeDirection(E_DIRECTION direction){
	this->direction = direction;
}

void cPlayer::setConnection(cConnection* connection){
	this->connection = connection;
}

cConnection * cPlayer::getConnection(){
	return connection;
}

std::string cPlayer::serialize(){
	long double temp_id = this->id;
	return 
		"object\tplayer\t"+std::to_string(temp_id)+/*tutaj jeszcze hashcode powinien byæ*/
		+"\nposition\t"+std::to_string((long double)pos.first) + "\t" + std::to_string((long double)pos.second);
}

