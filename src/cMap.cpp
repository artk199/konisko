#include "cMap.h"
#include "Assets.h"
#include <iostream>
#include <map>

cMap::cMap(){	
	przes=Vector2(0,0);
	//wczytanie zawartosci mapy
	int maxy=0, maxx=0;

	mapString ="XXXXXXXXXXX\nX         X\nX    X    X\nX    X    X\nX         X\nXXXXXXXXXXX";
	
	
	for(int i = 0 ;i < 6 ; i++){
		tiles.push_back(std::vector<spcTile>());
		for(int j = 0 ; j < 11 ;j++){
			if(mapString[i*11+j+i] == 'X'){
				spcTile nowy = new cTile("wall1",Vector2(j*64+przes.x,i*64+przes.y),this);
				nowy->setDestroyable(true);
				nowy->setWalkable(false);
				tiles[i].push_back(nowy);
			}else{
				spcTile nowy = new cTile("grass1",Vector2(j*64+przes.x,i*64+przes.y),this);
				nowy->setDestroyable(true);
				nowy->setWalkable(true);
				tiles[i].push_back(nowy);
			}	

		}
	}
	/*
	for(int i=0; i<Assets::gameMap.getCount()-1; i++){
		//pobranie wlasciwosci nowego pola
		Resource *kt=Assets::gameMap.get(i);
		int y=kt->getAttribute("y").as_int();
		int x=kt->getAttribute("x").as_int();
		string name=kt->getAttribute("type").as_string();
		bool destroyable = kt->getAttribute("destroyable").as_bool();
		bool walkable = kt->getAttribute("walkable").as_bool();

		//ustawienie wymiarow mapy
		if(y>maxy) maxy=y;
		if(x>maxx) maxx=x;

		if(name.length()>0){
			//stworzenie nowego pola
			spcTile nowy = new cTile(name,Vector2(x*64+przes.x,y*64+przes.y),this);
			nowy->setDestroyable(destroyable);
			nowy->setWalkable(walkable);

			tiles[make_pair(y,x)] = nowy;
		}
	}
	size = make_pair(maxy,maxx);*/
};

void cMap::clear(){
};