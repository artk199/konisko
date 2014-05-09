#pragma once
#include "oxygine-framework.h"

using namespace oxygine;

namespace Assets{
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