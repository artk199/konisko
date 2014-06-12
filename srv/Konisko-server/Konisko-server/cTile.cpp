#include "cTile.h"


cTile::cTile(void)
{
}

cTile::cTile(int type)
{
	if(type == 1)
		walkable = true;
	else
		walkable = false;
}


cTile::~cTile(void)
{
}

void cTile::setWalkable(bool s){
	walkable = s;
};
void cTile::setDestroyable(bool s){
	destroyable = s;
};
bool cTile::isWalkable(){
	return walkable;
};
bool cTile::isDestroyable(){
	return destroyable;
};