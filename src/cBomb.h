#pragma once 
#include "oxygine-framework.h"
#include "cUI.h"
#include "Assets.h"
#include "cSerializable.h"

DECLARE_SMART(cBomb, spcBomb);

class cBomb:public Actor, public cSerializable{
	spSprite bomb;
	spSprite fire;
	spSprite boom;


	int range;
	int pozx, pozy;

	public:
		cBomb(int x, int y);
		void destroy(); // - Uruchamia TweenAlpha z wybuchem
		void setRange(int range); // - Ustawia zasieg razenia bomby po wybuchu
};