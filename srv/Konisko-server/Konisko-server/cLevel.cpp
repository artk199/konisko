#include "cLevel.h"


cLevel::cLevel(void)
{
	connected_players = 0;
	lvl_state = PREPARATION;
}


cLevel::~cLevel(void)
{
}

//-- Dodanie gracza
void cLevel::addPlayer(cPlayer* player){
	
	if (connected_players >= N_OF_PLAYERS){
		throw "Maksymalna liczba graczy osi�gni�ta!";
	}
	if (lvl_state != PREPARATION){
		throw "Po rozpocz�ciu gry nie mo�na dodawa� graczy!";
	}
	
	this->players[connected_players] = player;
	connected_players++;

}

//--Ustawienie mapy
void cLevel::setMap(cMap* map){

	if(lvl_state != PREPARATION){
		throw "B��d przy ustawaniu mapy, gra ju� rozpocz�ta.";
	}

	this->map = map;
}

//--P�tla g��wna gry
void cLevel::start(){
	
	lvl_state = ACTIVE;

	//Zmienna kontroluj�ca czy gra si� zako�czy�a
	bool is_active = true;

	//Delta czasu
	double delta = 0;

	while(is_active){
		
		//Ograniczy� do max 60 obrat�w na sekund�
		//10 przyk�adowo dane na szybko
		delta = 10;
		
		map->update(delta);
	
		for(int i=0;i<connected_players;i++){
			players[i]->update(delta);
		}

	
	}

}
