#include "cMap.h"
#include "Assets.h"
#include <iostream>
#include <map>

cMap::cMap(){	
	przes=Vector2(0,0);
	//wczytanie zawartosci mapy
	int maxy=0, maxx=0;

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

		if( name.length()>0){
			//stworzenie nowego pola
			spcTile nowy = new cTile(name,Vector2(x*64+przes.x,y*64+przes.y),this);
			nowy->setDestroyable(destroyable);
			nowy->setWalkable(walkable);

			tiles[make_pair(y,x)] = nowy;
		}
	}
	size = make_pair(maxy,maxx);
};

void cMap::clear(){
	auto it=tiles.begin();
	auto end = tiles.end();

	while(it!=end){
		std::cout<<it->second->getSprite()->_ref_counter<<endl;
		it->second->releaseRef();
		it++;
	}
};