#pragma once
#include "oxygine-framework.h"
#include "cUnit.h"

using namespace oxygine;

class cPlayer : public cUnit{
public :
	cPlayer(Vector2 pos);
	void _init();
	void move(Vector2 delta);
private :
	spSprite sprite;
	Vector2 pos;
};

