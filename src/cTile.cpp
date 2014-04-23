#include "cTile.h"
#include "Assets.h"
#include <iostream>

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
