#pragma once
#include <string>
#include <vector>
#include "cTile.h"

class cGame;
class cMap
{
public:
	cMap(void);
	cMap(int type);
	~cMap(void);
	bool canMove(double,double);
	void update(double delta);
	bool isMoveable(double x, double y);
	std::pair<int,int> getPositionOnMap(double,double);

	bool destroyTile(int x, int y, cGame *g); // daje true jesli ogien ma leciec dalej
	void setGame(cGame *g);
private:
	std::vector<std::vector<cTile*>> tiles;
	std::string mapString;
	cGame *game;
};

