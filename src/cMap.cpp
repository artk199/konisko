#include "cMap.h"
#include "Assets.h"
#include <iostream>

cMap::cMap(cGame *game)
{	
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
		
	}
}
