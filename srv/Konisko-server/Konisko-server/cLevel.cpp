#include "cLevel.h"
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <windows.h>
#include <omp.h>

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
	for(int i=0;i<connected_players;i++)
		players[i]->attachToMap(map);
}

//--P�tla g��wna gry
void cLevel::start(){
	
	lvl_state = ACTIVE;

	//Zmienna kontroluj�ca czy gra si� zako�czy�a
	bool is_active = true;

	//Delta czasu
	double delta = 0;
	int i = 0;
	while(is_active){
		
		//Ograniczy� do max 60 obrat�w na sekund�
		//10 przyk�adowo dane na szybko
		double start = omp_get_wtime();
		
		
		Sleep(1);
		double end = omp_get_wtime();
		//Zmierzony czas w sekundach
		delta = end - start;
		i++;

		map->update(delta);
	
		for(int i=0;i<connected_players;i++){
			players[i]->update(delta);
		}


		this->serializabled = this->serialize();
		//std::cout<<delta<<"\n"<<serializabled<<std::endl;
	
	}

}

std::string cLevel::serialize(){
	std::string r = "";

	for(int i=0;i<connected_players;i++)
		r += players[i]->serialize() + "\n";
	
	return r;
}