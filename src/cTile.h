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
	public:
		cTile(string n, Vector2 p, Actor *par);
		~cTile();
		
		void _init();

	protected:
		spSprite sprite;
		Vector2 pos;
		bool destroyable;
		string name;

	/*KOD ARTURA
public :
	cTile(Vector2 pos);
	~cTile(void);
	void _init();			//metoda virtualna z cUnit
	void setType(tileType);			//ustawia typ kloceczka
protected :

	spSprite sprite;
	Vector2 pos;
	tileType type;*/
};