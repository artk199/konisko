#include "cTile.h"
#include "Assets.h"
#include <iostream>


cTile::cTile(string n, Vector2 p, spActor par){
	name = n;
	pos = p;
	_view = par;

	//stworzenie sprite'a
	sprite = new Sprite;
	sprite->setResAnim(Assets::gameResources.getResAnim(name));
	sprite->setPosition(pos.x,pos.y);
	sprite->attachTo(_view);

	//ustawienie parametrow domyslnych
	walkable = false;
	destroyable = false;
};

void cTile::clear(){
	sprite->releaseRef();
};

cTile::~cTile(){
	clear();
};

void cTile::_init(){
	sprite = new Sprite;
	sprite->setResAnim(Assets::gameMap.getResAnim(name));
	sprite->setPosition(pos.x,pos.y);

	sprite->attachTo(_view);
};

//---FUNKCJE DOSTEPU
void cTile::setWalkable(bool s){walkable = s;};
void cTile::setDestroyable(bool s){destroyable = s;};
bool cTile::isWalkable(){return walkable;};
bool cTile::isDestroyable(){return destroyable;};
spSprite cTile::getSprite(){return sprite;};