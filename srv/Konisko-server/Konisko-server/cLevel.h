#pragma once
#include "cMap.h"
#include "cPlayer.h"
#include "cGame.h"


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
private:
	cMap* map;
	cPlayer* players[4];
	int connected_players;
	LVL_STATE lvl_state;
};

