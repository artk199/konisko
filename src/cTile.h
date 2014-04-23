#pragma once
#include "oxygine-framework.h"
#include "cUnit.h"

using namespace oxygine;
enum tileType{
	dirt,
	obsydian,
	none
};

class cTile: public cUnit{
public :
	cTile(Vector2 pos);
	~cTile(void);
	void _init();			//metoda virtualna z cUnit
	void setType(tileType);			//ustawia typ kloceczka
protected :
	spSprite sprite;
	Vector2 pos;
	tileType type;
};