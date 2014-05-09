#pragma once
#include "cTile.h"
#include <vector>
#include <utility>
#include <map>

using namespace std;

DECLARE_SMART(cMap, spcMap);

class cMap: public Actor{
	public:
		cMap();
		void clear();

		map<pair<int,int>, spcTile> tiles; // kontener ze wszystkimi polami
	private:
		
		pair<int,int> size; // rozmiary wczytanej mapy
		Vector2 przes; // wektor przesuniecia mapy wzgledem okna (margines lewy i gorny)
};