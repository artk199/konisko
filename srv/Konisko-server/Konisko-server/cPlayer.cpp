#include "cPlayer.h"
#include "cBomb.h"
#include "cLevel.h"

cPlayer::cPlayer(int ID)
{
	this->id=ID;
	
	this->direction = LEFT;
	this->new_direction = LEFT;
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
	//printf("Pozycja %d: [%f, %f]\n", id, y,x);
}

double cPlayer::round(double x1){	
	double x = x1 + 32;
	double result = x;
	double odleglosc = abs(fmod(x,64.0));
	if(odleglosc < 10.f || (64 - odleglosc) < 10.f ){
		double number = x/64;
		int temp = number < 0.0 ? ceil(number - 0.5) : floor(number + 0.5);
		result = 64 * temp;
	}
	return result - 32;
}
void cPlayer::move(double delta){


	double  newX = this->pos.first,
			newY = this->pos.second;
	if(direction != new_direction){
			switch (new_direction){
				case TOP:
					newY = this->pos.second - delta * this->velocity;
				break;
				case BOT:
					newY = this->pos.second + delta * this->velocity;
				break;
				case LEFT:
					newX = this->pos.first - delta * this->velocity;
				break;
				case RIGHT:
					newX = this->pos.first + delta * this->velocity;
				break;
			}
			if(new_direction == TOP || new_direction == BOT){
				newX = round(newX);
			}else{
				newY = round(newY);
			}
			if(map->canMove(newX,newY)){
				direction = new_direction;
				this->pos.first = newX;
				this->pos.second = newY;
				return;
			}
	}

	newX = this->pos.first;
	newY = this->pos.second;

	switch (direction){
		case TOP:
			newY = this->pos.second - delta * this->velocity;
		break;
		case BOT:
			newY = this->pos.second + delta * this->velocity;
		break;
		case LEFT:
			newX = this->pos.first - delta * this->velocity;
		break;
		case RIGHT:
			newX = this->pos.first + delta * this->velocity;
		break;
	}
	if (map->canMove(newX,newY)){
		this->pos.first = newX;
		this->pos.second = newY;
	}else{
		this->pos.first = round(this->pos.first);
		this->pos.second = round(this->pos.second);
		direction = STAND;
	}

	//printf("Pozycja %d: [%f, %f]\n", id, pos.second,pos.first);
}

void cPlayer::update(double delta){
	this->move(delta);
	for(int i=0;i<max_bomb;i++){
		if(bombs[i] != NULL){
			bombs[i]->update(delta);
			if(bombs[i]->isDestroyed()){
				lvl->BOOM(bombs[i]->getPosition(),1);
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
	this->new_direction = direction;
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

//---Informuje czy ktorakolwiek bomba gracza znajduje sie na tym polu
bool cPlayer::isAnyBombThere(int y, int x){
	for(int i=0; i<bombs.size(); i++){
		if(bombs[i]!=NULL){
			pair<int,int> pos = bombs[i]->getPosition();
			if(x==pos.first && y == pos.second) return true;
		}
	}
	return false;
};