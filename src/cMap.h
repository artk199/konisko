#pragma once
#include "cTile.h"
#include <vector>
#include <utility>

;using namespace std;

class cMap
{

	public :
		cMap(cGame *game);
	private:
		pair<int,int> size;
		vector<vector<cTile*>> tiles;
};