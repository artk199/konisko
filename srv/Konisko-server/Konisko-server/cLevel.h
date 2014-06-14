#pragma once
#include "cMap.h"
#include "cPlayer.h"
#include "cGame.h"
#include <string>
#include <list>
#include "cBomb.h"

enum LVL_STATE{
	PREPARATION,
	ACTIVE
};

class cLevel
{
public:
	cLevel(void);
	~cLevel(void);
	void start();
	void addPlayer(cPlayer *player);
	void setMap(cMap* map);
	std::string serialize();
	std::string getSerialized(){return this->serializabled;};
	void setBomb(int);
	void BOOM(std::pair<int,int>,int);
	void addGame(cGame*);
private:
	std::string serializabled;
	cMap* map;
	cPlayer* players[4];
	int connected_players;
	LVL_STATE lvl_state;
	cGame* game;
};

