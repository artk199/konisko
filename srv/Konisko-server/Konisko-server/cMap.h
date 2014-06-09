#pragma once
#include <string>
#include <vector>
#include "cTile.h"


class cMap
{
public:
	cMap(void);
	cMap(int type);
	~cMap(void);
	bool canMove(double,double);
	void update(double delta);
	bool isMoveable(double x, double y);
private:
	std::vector<std::vector<cTile*>> tiles;
	std::string mapString;
};

