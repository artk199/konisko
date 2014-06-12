#pragma once
#include "cPlayer.h"
#include <string>

class cBomb
{
public:
	cBomb(void);
	~cBomb(void);
	bool isDestroyed();
	void update(double delta);
	void setPosition(int,int);
	void setPosition(std::pair<int,int>);
	string serialize();
	int getId(){return id;};
	void setId(int id){this->id = id;};
	std::pair<int,int> getPosition();
private:
	std::pair<int,int> pos;
	double time;
	int id;
	int power;
	cPlayer* owner;
};

