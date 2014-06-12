#include "Assets.h"
#include <fstream>
#include <iostream>
#include "oxygine-framework.h"

using namespace oxygine;

//---Sprawdza, czy podany string znajduje sie w drugim stringu od danego miejsca
bool sprawdzString(string napis, string szukaj, int &poz){
	if(poz<0) return false;

	int d1=napis.length();
	int d2=szukaj.length();

	//jezeli szukany napis nie miesci sie w stringu funkcji
	if(d1-poz<d2) return false;

	int j=0;
	int i;
	for(i=poz; i<d1 && j<d2; i++, j++)
		if(napis[i]!=szukaj[j]) return false;
	
	poz = i;

	return true;

};
namespace Assets
{
	Resources gameResources;
	Resources gameMap;
	Vector2 windowSize;
	string userNick = "New player";
	string serverName = "localhost";
	string serverPort = "27015";
	map <int, string> errors;
	 
	string settingsFile="settings.xml";
	
	void loadSettingsFromFile(){
		ifstream plik;
		plik.open(settingsFile);
		if(plik.is_open()){
			while(plik.good()){
				char linia[512];
				plik.getline(linia,512);
				string str = linia;
				int i=0;
				if(sprawdzString(str,"\t<usernick>",i)){
					string wartosc="";
					while(str[i]!='<') wartosc+=str[i++];
					userNick = wartosc;
				}
				i=0;
				if(sprawdzString(str,"\t<servername>",i)){
					string wartosc="";
					while(str[i]!='<') wartosc+=str[i++];
					serverName = wartosc;
				}
				i=0;
				if(sprawdzString(str,"\t<serverport>",i)){
					string wartosc="";
					while(str[i]!='<') wartosc+=str[i++];
					serverPort = wartosc;
				}
			}
		}

		plik.close();
	}



	void saveSettingsToFile(){
		ofstream plik;
		plik.open(settingsFile);
		plik<<"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"<<endl;
		plik<<"<settings>"<<endl;

		plik<<"\t<usernick>"<<userNick<<"</usernick>"<<endl;
		plik<<"\t<servername>"<<serverName<<"</servername>"<<endl;
		plik<<"\t<serverport>"<<serverPort<<"</serverport>"<<endl;

		plik<<"</settings>"<<endl;

		plik.close();
	}



	void load(){
		//load our resources
		gameResources.loadXML("res.xml");

		//zaladowanie mapy
		gameMap.loadXML("map.xml");

		//Wpisze siê tutaj rozmiar okna
		windowSize = Vector2(960,640);

		loadErrors();
	};

	void free(){
		//unload
		gameResources.free();
		gameMap.free();
	};
		

	void loadErrors(){
		errors[0] = "Invalid server adress. Use IP or localhost!";
		errors[1] = "Unable to connect to server. Check server adress!";
	};
};