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
	float startTime;
	float destroyTime;

	bool destroyed;

	public:
		cBomb(int x, int y, float time=0);
		~cBomb();

		void destroy(); // - Uruchamia TweenAlpha z wybuchem
		void setRange(int range); // - Ustawia zasieg razenia bomby po wybuchu
		float getTime(); // - Zwraca czas rozstawienia bomby
		void setDestroyTime(float t); // - Ustawia czas, po jakim bomba wybucha
		float getDestroyTime(); // - Zwraca czas, po jakim bomba wybucha
		bool updateDestroyTime(int dt); // - Odejmuje od czasu wybuchu dt i wybucha po uplywie czasu (zwraca wtedy true)
		bool isDestroyed(){return destroyed;};
};