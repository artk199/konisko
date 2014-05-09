#pragma once
#include "oxygine-framework.h"
#include "cUnit.h"

using namespace oxygine;

DECLARE_SMART(cPlayer, spcPlayer);

class cPlayer : public cUnit{
	public :
		cPlayer(Vector2 pos);
		void _init();
		void move(Vector2 delta);

		string getName();
		void setName(string n);
		spSprite getIcon();

	private:
		spSprite sprite;
		Vector2 pos;
		string name;
};

