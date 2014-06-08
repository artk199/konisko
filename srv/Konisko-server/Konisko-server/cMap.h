#pragma once
#include <string>
#include <vector>
#include "cTile.h"

#define TILE_SIZE 64;
class cMap
{
public:
	cMap(void);
	cMap(int type);
	~cMap(void);
	bool canMove(double,double);
	void update(double delta);
private:
	std::vector<std::vector<cTile*>> tiles;
};

