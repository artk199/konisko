#pragma once
#include <string>
#include "cConnection.h"

using namespace std;

class cPlayer
{
public:
	cPlayer(void);
	~cPlayer(void);
	void setNick(string nick);
	string getNick();
	void setConnection(cConnection* c);
	void update(double delta);
	double getPosX();
	double getPosY();
	void setPos(double x,double y);
	double getVelocity();
	void setVelocity(double velocity);
	void move();
	std::pair<double,double> getNextPos(double delta);
private:
	//Nick gracza
	string nick;
	//Dane o po³¹czeniu
	cConnection* connection;
	
	//Dane do gry
	pair<double,double> pos;
	double velocity;

};

