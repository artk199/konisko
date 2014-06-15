#pragma once
#include <string>
#include "cConnection.h"
#include "cMap.h"
#include <vector>
#include "cGame.h"

class cLevel;
class cBomb;

using namespace std;

enum E_DIRECTION{TOP=1,BOT,LEFT,RIGHT,STAND};
enum E_PLAYER_STATE{PLANTING,GOING,DEAD};

class cPlayer
{
	public:
		cPlayer(void);
		~cPlayer(void);
		void setNick(string nick);
		string getNick(){return nick;};
		void setConnection(connection* c);
		connection * getConnection();
		void setLvl(cLevel* l){this->lvl = l;};
		void update(double delta);
		double getPosX(){return pos.first;};
		double getPosY(){return pos.second;};
		void setPos(double x,double y);
		double getVelocity();
		void setVelocity(double velocity);
		void move(double delta);
		std::pair<double,double> getNextPos(double delta);
		void attachToMap(cMap* map);
		void changeDirection(E_DIRECTION direction);
		string serialize();
		int id;
		void addBomb();

		bool isAnyBombThere(int y, int x); // - Informuje czy ktorakolwiek bomba gracza znajduje sie na tym polu
	private:
		//Nick gracza
		string nick;
	
		//Dane o po³¹czeniu
		connection* c;
		bool ready;
		int ilosc_bomb;
		int max_bomb;
		//Dane do gry
		pair<double,double> pos;
		double velocity;
		cMap* map;
		cLevel* lvl;
		E_DIRECTION direction;
		std::vector<cBomb*> bombs;

};

