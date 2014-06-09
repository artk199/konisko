#include "cTile.h"


cTile::cTile(void)
{
}

cTile::cTile(int type)
{
	if(type == 1)
		moveable = true;
	else
		moveable = false;
}


cTile::~cTile(void)
{
}

bool cTile::isMoveable(){
	return moveable;
}