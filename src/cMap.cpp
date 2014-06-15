#include "cMap.h"
#include "Assets.h"
#include <iostream>
#include <map>

cMap::cMap(){	
	przes=Vector2(0,0);
	//wczytanie zawartosci mapy
	int maxy=0, maxx=0;

	mapString ="XXXXXXXXXXXXXXX\nX  YYYYYYYYY  X\nX XYXYXYXYXYX X\nX YYYYYYYYYYY X\nXYXYXYXYXYXYXYX\nX YYYYYYYYYYY X\nXYXYXYXYXYXYXYX\nX YYYYYYYYYYY X\nX YYXYXYXYXYY X\nXXXXXXXXXXXXXXX\n";
	
	int y=0, x=0;
	tiles.push_back(std::vector<spcTile>());
	for(int i =0; i<mapString.length(); i++){
		//wykryto nowa linie
		if(mapString[i]=='\n'){
			tiles.push_back(std::vector<spcTile>());
			y++;
			x=0;
		}
		else{
			//rozpoznanie pola
			if(mapString[i] == 'X'){
				spcTile nowy = new cTile("wall4",Vector2(x*64+przes.x,y*64+przes.y),this);
				nowy->setDestroyable(false);
				nowy->setWalkable(false);
				tiles[y].push_back(nowy);
			}
			else if(mapString[i] == 'Y'){
				spcTile nowy = new cTile("wall2",Vector2(x*64+przes.x,y*64+przes.y),this);
				nowy->setDestroyable(true);
				nowy->setWalkable(false);
				tiles[y].push_back(nowy);
			}
			else{
				spcTile nowy = new cTile("grass1",Vector2(x*64+przes.x,y*64+przes.y),this);
				nowy->setDestroyable(true);
				nowy->setWalkable(true);
				tiles[y].push_back(nowy);
			}
			x++;
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

//---Niszczy dana komorke na mapie
void cMap::destroy(int y, int x){
	tiles[y][x]->destroy();
}; 

void cMap::updateMap(string newmap){
	//std::cout<<"Aktualizuje: \n"<<newmap<<endl;
	//std::cout<<"STARA: \n"<<mapString<<endl;
	//jezeli nie ma bledu w transmisji
	if(mapString.length() == newmap.length()-2){
		for(int i=0; i<mapString.length(); i++){
			//jest roznica w polach
			if(mapString[i]!=newmap[i]){
				int x, y;
				y=floor(1.0*i/tiles[0].size());
				x=i-y*tiles[0].size()-y;
				tiles[y][x]->destroy();

				mapString[i] = newmap[i];
			}
		}
	}
};