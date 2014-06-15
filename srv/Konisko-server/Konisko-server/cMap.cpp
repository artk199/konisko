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
	mapString ="XXXXXXXXXXXXXXX\nX  YYYYYYYYY  X\nX XYXYXYXYXYX X\nX YYYYYYYYYYY X\nXYXYXYXYXYXYXYX\nX YYYYYYYYYYY X\nXYXYXYXYXYXYXYX\nX YYYYYYYYYYY X\nX YYXYXYXYXYY X\nXXXXXXXXXXXXXXX\n";
	
	int y=0, x=0;
	tiles.push_back(std::vector<cTile*>());
	for(int i =0; i<mapString.length(); i++){
		//wykryto nowa linie
		if(mapString[i]=='\n'){
			tiles.push_back(std::vector<cTile*>());
			y++;
			x=0;
		}
		else{
			//rozpoznanie pola
			if(mapString[i] == 'X')
				tiles[y].push_back(new cTile(2));		
			else if(mapString[i] == 'Y')
				tiles[y].push_back(new cTile(3));			
			else
				tiles[y].push_back(new cTile(1));	

			tiles[y][x]->setPos(x,y);
			x++;
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
		if(tiles[y][x]->destroy(g))
			return true;	
		//szansa ze cos uleglo zniszczeniu
		else{
			if(mapString[y*tiles[0].size()+x+y]=='Y')
				mapString[y*tiles[0].size()+x+y] = ' ';
			return false;
		}
}; 

void cMap::setGame(cGame *g){game=g;}

std::string cMap::getMap(){return mapString;};