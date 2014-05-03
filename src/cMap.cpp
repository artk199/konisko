#include "cMap.h"
#include "Assets.h"
#include <iostream>
#include <map>

cMap::cMap(){	
	przes=Vector2(100,100);
	//wczytanie zawartosci mapy
	int maxy=0, maxx=0;
	for(int i=1; i<Assets::gameMap.getCount(); i++){
		//pobranie wlasciwosci nowego pola
		Resource *kt=Assets::gameMap.get(i);
		int y=kt->getAttribute("y").as_int();
		int x=kt->getAttribute("x").as_int();
		string name= kt->getName();
		string type=kt->getAttribute("type").as_string();
		bool destroyable = kt->getAttribute("destroyable").as_bool();
		bool walkable = kt->getAttribute("walkable").as_bool();

		//ustawienie wymiarow mapy
		if(y>maxy) maxy=y;
		if(x>maxx) maxx=x;

		//stworzenie nowego pola
		tiles[make_pair(y,x)] = new cTile(name,Vector2(x*64+przes.x,y*64+przes.y),this);
		//nowy->init(game);
	}
	size = make_pair(maxy,maxx);

	printf("WYMIARY: %d, %d", size.first,size.second);
	/* KOD ARTURA
	//wczytanie mapy i wpisanie jej to tilesów
	for(int i=0;i<Assets::mapString.size();i++){
		tiles.push_back(vector<cTile*>());
		std::cout<<Assets::mapString[i]<<endl;
		for(int j=0;j<Assets::mapString[i].length();j++){
			tiles[i].push_back(new cTile(Vector2(j*64,i*64)));
			switch(Assets::mapString[i][j]){
			case 'X':
			case 'x':
				tiles[i][j]->setType(obsydian);
				break;
			case '.':
				tiles[i][j]->setType(dirt);
				break;
			default:
				tiles[i][j]->setType(none);
				break;
			}
			tiles[i][j]->init(game);
		}
		
	}KOD ARTURA */

};

void cMap::clear(){
/*	auto it = tiles.begin();
	int i=0, d=tiles.size();
	while(i<d){
		delete it->second;
		it++;
		i++;
	}
	tiles.clear();*/
};
