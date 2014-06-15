#include "cTile.h"
#include "cGame.h"

cTile::cTile(void)
{
}

cTile::cTile(int type)
{

	switch (type){
		case 1:
			walkable = true;
			destroyable = false;
		break;
		case 2:
			walkable = false;
			destroyable = false;
		break;
		case 3:
			walkable = false;
			destroyable = true;
		break;
	}
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

 // zwraca true, jezeli ogien ma isc dalej
bool cTile::destroy(cGame *g){
	//mo¿na zniszczyc
	if(destroyable){
		destroyable = false;
		walkable = true;

		string poz;
		poz+=to_string(long double(pozy))+"\t";
		poz+=to_string(long double(pozx))+"\t";
		//g->sendToClient(NULL,DESTROY,poz);
		return false;	
	}
	//trafiono na sciane
	if(!walkable){
		return false;
	}
	return true;
};

void cTile::setPos(int x, int y){
	pozx = x;
	pozy = y;
}