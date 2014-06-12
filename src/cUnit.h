#pragma once
#include "oxygine-framework.h"
using namespace oxygine;

class cGame;

class cUnit: public Object
{
public:
	cUnit();

	void init(cGame *game);

	void update(const UpdateState &us);
	virtual void deserialize(string s){};
protected:
	virtual void _init(){}
	virtual void _update(const UpdateState &us){}

	spActor _view;
	cGame *_game;
};