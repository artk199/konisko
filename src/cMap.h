#pragma once
#include "cTile.h"
#include <vector>
#include <utility>
#include <map>

using namespace std;

class cMap: public Actor{
	public:
		cMap();
		void clear(); // - znaleziono wyciek pamieci przy kilkukrotnym uruchomieniu nowej gry, sprawdzic
		//cMap(cGame *game);
	private:
		map<pair<int,int>, cTile *> tiles;
		pair<int,int> size;
		Vector2 przes;
};