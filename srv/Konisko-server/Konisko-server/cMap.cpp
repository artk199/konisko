#include "cMap.h"
#include <iostream>

#define TILE_SIZE 64


cMap::cMap(void)
{
}

cMap::cMap(int type)
{
	mapString ="XXXXXXXXXXX\nX         X\nX    X    X\nX    X    X\nX         X\nXXXXXXXXXXX";
	
	for(int i = 0 ;i < 6 ; i++){
		tiles.push_back(std::vector<cTile*>());
		for(int j = 0 ; j < 11 ;j++){
			if(mapString[i*11+j+i] == 'X')
				tiles[i].push_back(new cTile(2));		
			else
				tiles[i].push_back(new cTile(1));		
			
		}
	}
/*	
	std::vector<std::vector<cTile*>>::iterator it = tiles.begin();
	std::vector<std::vector<cTile*>>::iterator end = tiles.end();
	while(it != end){
		std::vector<cTile*>::iterator it2 = (*it).begin();
		std::vector<cTile*>::iterator end2 = (*it).end();
		while(it2 != end2){
			(*it2) = new cTile();
			it2++;
		}
		it++;
	}
*/
}

cMap::~cMap(void)
{
}

bool cMap::isMoveable(double x, double y){
	//std::cout<<floor(x/TILE_SIZE)<<" "<<floor(y/TILE_SIZE)<<std::endl;
	if(y>=0 && std::floor(y/TILE_SIZE) < tiles.size()
	&& x>=0 && std::floor(x/TILE_SIZE) < tiles[std::floor(y/TILE_SIZE)].size())
		return tiles[std::floor(y/TILE_SIZE)][std::floor(x/TILE_SIZE)]->isWalkable();
	return false;
}

bool cMap::canMove(double x, double y){
	//x oraz y to pkt w pikselach srodek gdzie sie znajduje
	/*------ ------
	 |  *------*   |
	 |  |      |   |
      --|      |---
	 |  *------*   |
	 |      |      |
	  ------ ------				
	*/
	//std::cout<<x<<" "<<y<<std::endl;
	if(!isMoveable(x-TILE_SIZE/2,y-TILE_SIZE/2))
		return false;
	if(!isMoveable(x+TILE_SIZE/2-1,y-TILE_SIZE/2))
		return false;
	if(!isMoveable(x-TILE_SIZE/2,y+TILE_SIZE/2-1))
		return false;
	if(!isMoveable(x+TILE_SIZE/2-1,y+TILE_SIZE/2-1))
		return false;
	return true;
}

void cMap::update(double delta){
	
}

std::pair<int,int> cMap::getPositionOnMap(double posx, double posy){
	int x = std::floor(posx / TILE_SIZE);
	int y = std::floor(posy / TILE_SIZE);
	return std::pair<int,int>(x,y);
}