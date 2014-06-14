#pragma once
#include "oxygine-framework.h"

using namespace oxygine;

namespace Assets{
	enum REQUESTS{ILOSC_GRACZY=33, KONIEC=34, DELTA=35, PLAYER_POSITION=36, UPDATE_ALL=37, START_GAME=38, SET_PLAYER_ID = 39,
	PLAYER_JOINED=40, PLAYER_QUIT=41, KEY_PRESSED=42, CONNECT=43};

	extern Resources gameResources;
	extern Resources gameMap;
	extern Vector2 windowSize;
	extern string userNick;
	extern string serverName;
	extern string serverPort;
	extern string settingsFile;
	void load();
	void free();

	
	void loadSettingsFromFile();
	void saveSettingsToFile();

	extern map <int, string> errors;
	void loadErrors();
};