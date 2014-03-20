#pragma once
#include "oxygine-framework.h"
using namespace oxygine;

//---KLASA ZARZADZAJACA DANYMI WCZYTYWANYMI Z ZEWNATRZ
static class cAssets{
	public:
		 static Resources gameResources;
		
		 static void LoadResources();
};

