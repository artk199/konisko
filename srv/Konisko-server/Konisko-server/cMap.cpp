#include "cMap.h"


cMap::cMap(void)
{
}

cMap::cMap(int type)
{
	std::vector<std::vector<cTile*>>::iterator it = tiles.begin();
	std::vector<std::vector<cTile*>>::iterator end = tiles.end();
	while(it != end){
		std::vector<cTile*>::iterator it2 = (*it).begin();
		std::vector<cTile*>::iterator end2 = (*it).end();
		while(it2 != end2){
			(*it2) = new cTile();
			it2++;
		}
		it++;
	}
}

cMap::~cMap(void)
{
}

bool cMap::canMove(double x, double y){
	
	return true;
}

void cMap::update(double delta){
	
}
