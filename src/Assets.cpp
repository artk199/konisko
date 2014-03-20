#include "Assets.h"

namespace Assets
{
	Resources gameResources;

	void load()
	{
		//load our resources
		gameResources.loadXML("res.xml");
	}

	void free()
	{
		//unload
		gameResources.free();
	}
}