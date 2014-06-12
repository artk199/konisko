#pragma once
#include "oxygine-framework.h"
#include "cUnit.h"
#include "cSerializable.h"
#include "cBomb.h"

using namespace oxygine;

DECLARE_SMART(cPlayer, spcPlayer);

class cPlayer : public Actor, public cSerializable{
	public :
		cPlayer();

		string &getNick(){return nick;};
		void setNick(string n){nick=n;};
		void setID(int i){id = i;}
		int & getID(){return id;};
		spSprite getIcon();
		void deserialize(string s);
		void move(Vector2 delta);
		void setPosition(double x, double y);

		void init();
		void setReady(bool flag){ready=flag;};
		bool &getReady(){return ready;};

		void updateBombs(int dt); // - Aktualizuje bomby i wybucha te, na ktore przyszedl juz czas, by oposcic ten ziemski padol
		void addBomb(int id, int x, int y, int range=1, int destroying_time=3000); // - Dodaje nowa bombe dla gracza pod jej adresem ID

	private:
		spSprite sprite;
		Vector2 pos, poprzpoz;
		int kierunek; // 0 gora 1 prawo 2 dol 3 lewo

		string nick;
		int id;
		bool visible;
		bool ready;
		vector <cBomb *> bombs;
};

