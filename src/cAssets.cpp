#include "oxygine-framework.h"
#include "cAssets.h"

using namespace oxygine;

Resources cAssets::gameResources;

void cAssets::LoadResources(){
	gameResources.loadXML("res.xml");
}