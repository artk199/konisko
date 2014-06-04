#include "Assets.h"
#include <fstream>
#include <iostream>
#include "oxygine-framework.h"

using namespace oxygine;

namespace Assets
{
	Resources gameResources;
	Resources gameMap;
	Vector2 windowSize;
	string userNick = "New player";
	string serverName = "localhost";
	map <int, string> errors;

	

	void load(){
		//load our resources
		gameResources.loadXML("res.xml");

		//zaladowanie mapy
		gameMap.loadXML("map.xml");

		//Wpisze siê tutaj rozmiar okna
		windowSize = Vector2(600,800);

		loadErrors();
	};

	void free(){
		//unload
		gameResources.free();
		gameMap.free();
	};
		

	void loadErrors(){
		errors[0] = "Invalid server adress. Use IP or localhost!";
		errors[1] = "Unable to connect to server. Check server adress!";
		errors[2] = "Disconnected from server...";
	};
};