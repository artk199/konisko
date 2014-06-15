#include "cMap.h"
#include <iostream>
#include "cGame.h"
#define TILE_SIZE 64
#include "cPlayer.h"

cMap::cMap(void)
{
}

cMap::cMap(int type)
{
	mapString ="XXXXXXXXXXX\nX         X\nX    Y    X\nX    Y    X\nX    Y    X\nXXXXXXXXXXX";
	
	for(int i = 0 ;i < 6 ; i++){
		tiles.push_back(std::vector<cTile*>());
		for(int j = 0 ; j < 11 ;j++){
			if(mapString[i*11+j+i] == 'X')
				tiles[i].push_back(new cTile(2));	
			else if(mapString[i*11+j+i] == 'Y')
				tiles[i].push_back(new cTile(3));		
			else
				tiles[i].push_back(new cTile(1));		
			tiles[i][j]->setPos(j,i);
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
	int py =std::floor(y/TILE_SIZE) ;
	int px = std::floor(x/TILE_SIZE);

	// sprawdzenie, czy na tym polu znajduje sie bomba
	for(int i=0; i<game->numberOfPlayers; i++){
		if(game->players[i]!=NULL && game->players[i]->isAnyBombThere(py,px)){
			int ppx = floor(game->players[i]->getPosX()/TILE_SIZE);
			int ppy = floor(game->players[i]->getPosY()/TILE_SIZE);
			//jezeli gracz znajduje sie na bombie, moze z niej zejsc
			if(px!=ppx && py!=ppy) 
				return false;
		}
	}
	

	if(y>=0 && py < tiles.size()
	&& x>=0 && px < tiles[0].size())
		return tiles[py][px]->isWalkable();
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

	if(!isMoveable(x-TILE_SIZE/2+1,y-TILE_SIZE/2+1))
		return false;
	if(!isMoveable(x+TILE_SIZE/2-1,y-TILE_SIZE/2+1))
		return false;
	if(!isMoveable(x-TILE_SIZE/2+1,y+TILE_SIZE/2-1))
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

// daje true jesli ogien ma leciec dalej
bool cMap::destroyTile(int x, int y, cGame *g){
	if(y>=0 && x>=0 && y<tiles.size() && x<tiles[0].size())
		return tiles[y][x]->destroy(g);
}; 

void cMap::setGame(cGame *g){game=g;}