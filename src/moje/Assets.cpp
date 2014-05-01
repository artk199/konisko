#include "Assets.h"
#include <fstream>
#include <iostream>
#include "oxygine-framework.h"

using namespace oxygine;

namespace Assets
{
	Resources gameResources;
	vector<string> mapString;		
	Vector2 windowSize;
	void load()
	{
		//load our resources
		gameResources.loadXML("res.xml");



		//load map from file
		ifstream file;
		file.open("map.dat");
		if (!file.is_open()){
			cout<< "Blad podczas ladowania danych mapy z pliku." <<endl;
			return;
		}
	
		while(!file.eof()){
			string temp;
			file>>temp;
			mapString.push_back(temp);
		}


		file.close();

		//Wpisze siê tutaj rozmiar okna
		windowSize = Vector2(100,100);
	}

	void free()
	{
		//unload
		gameResources.free();
	}
}