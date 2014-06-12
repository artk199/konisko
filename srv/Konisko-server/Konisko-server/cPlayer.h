#pragma once
#include <string>
#include "cConnection.h"
#include "cMap.h"
#include <vector>
class cBomb;

using namespace std;

enum E_DIRECTION{TOP,BOT,LEFT,RIGHT};
enum E_PLAYER_STATE{PLANTING,GOING,DEAD};

class cPlayer
{
	public:
		cPlayer(void);
		~cPlayer(void);
		void setNick(string nick);
		string getNick(){return nick;};
		void setConnection(cConnection* c);
		cConnection * getConnection();

		void update(double delta);
		double getPosX();
		double getPosY();
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
	private:
		//Nick gracza
		string nick;
	
		//Dane o połączeniu
		cConnection* connection;
		bool ready;
		int ilosc_bomb;
		int max_bomb;
		//Dane do gry
		pair<double,double> pos;
		double velocity;
		cMap* map;
		E_DIRECTION direction;
		std::vector<cBomb*> bombs;

};

