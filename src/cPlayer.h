#pragma once
#include "oxygine-framework.h"
#include "cUnit.h"
#include "cSerializable.h"
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

	private:
		spSprite sprite;
		Vector2 pos;
		string nick;
		int id;
		bool visible;
		bool ready;
};

