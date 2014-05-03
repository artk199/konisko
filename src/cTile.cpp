#include "cTile.h"
#include "Assets.h"
#include <iostream>


cTile::cTile(string n, Vector2 p, Actor *par){
	name = n;
	pos = p;
	_view = par;

	//stworzenie sprite'a
	sprite = new Sprite;
	sprite->setResAnim(Assets::gameMap.getResAnim(name));
	sprite->setPosition(pos.x,pos.y);
	sprite->attachTo(_view);
};

cTile::~cTile(){
	sprite->releaseRef();
};

void cTile::_init(){
	sprite = new Sprite;
	sprite->setResAnim(Assets::gameMap.getResAnim(name));
	sprite->setPosition(pos.x,pos.y);

	sprite->attachTo(_view);
};

/*KOD ARTURA
cTile::cTile(Vector2 pos){
	this->pos = pos;
}
void cTile::setType(tileType type){
	this->type = type;
}
void cTile::_init(){
	sprite = new Sprite;
	switch(type){
		case dirt:
			sprite->setResAnim(Assets::gameResources.getResAnim("tile1"));
			break;
		case obsydian:
			sprite->setResAnim(Assets::gameResources.getResAnim("tile2"));
			break;
		default:
			sprite->setResAnim(Assets::gameResources.getResAnim("tile1"));
	}
	sprite->setPosition(pos.x,pos.y);
	sprite->attachTo(_view);
}
cTile::~cTile()
{
}
KOD ARTURA*/