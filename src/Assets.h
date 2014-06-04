#pragma once
#include "oxygine-framework.h"

using namespace oxygine;

namespace Assets{
	enum REQUESTS{ILOSC_GRACZY=1, KONIEC=2, DELTA=3, PLAYER_POSITION=4, UPDATE_ALL=5};

	extern Resources gameResources;
	extern Resources gameMap;
	extern Vector2 windowSize;
	extern string userNick;
	extern string serverName;
	
	void load();
	void free();

	extern map <int, string> errors;
	void loadErrors();
};