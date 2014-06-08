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
		throw "Maksymalna liczba graczy osi¹gniêta!";
	}
	if (lvl_state != PREPARATION){
		throw "Po rozpoczêciu gry nie mo¿na dodawaæ graczy!";
	}
	
	this->players[connected_players] = player;
	connected_players++;

}

//--Ustawienie mapy
void cLevel::setMap(cMap* map){

	if(lvl_state != PREPARATION){
		throw "B³¹d przy ustawaniu mapy, gra ju¿ rozpoczêta.";
	}

	this->map = map;
}

//--Pêtla g³ówna gry
void cLevel::start(){
	
	lvl_state = ACTIVE;

	//Zmienna kontroluj¹ca czy gra siê zakoñczy³a
	bool is_active = true;

	//Delta czasu
	double delta = 0;

	while(is_active){
		
		//Ograniczyæ do max 60 obratów na sekundê
		//10 przyk³adowo dane na szybko
		delta = 10;
		
		map->update(delta);
	
		for(int i=0;i<connected_players;i++){
			players[i]->update(delta);
		}

	
	}

}
