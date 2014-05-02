#pragma once
#include "oxygine-framework.h"

using namespace oxygine;

namespace Assets{
	extern Resources gameResources;
	extern vector<std::string> mapString;
	extern Vector2 windowSize;
	extern string userNick;
	extern string serverName;

	void load();
	void free();
};