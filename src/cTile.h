#pragma once
#include "oxygine-framework.h"
#include "cUnit.h"

using namespace oxygine;

DECLARE_SMART(cTile, spcTile);

class cTile: public cUnit{
	public:
		cTile(string n, Vector2 p, spActor par);
		~cTile();
		
		void _init();
		void clear();

		//funkcje dostepu
		void setWalkable(bool s); 
		void setDestroyable(bool s);
		bool isWalkable();
		bool isDestroyable();
		spSprite getSprite();

		void destroy();
	protected:
		spSprite sprite;
		Vector2 pos;
		bool destroyable;
		bool walkable;
		string name;
};