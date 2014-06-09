#pragma once
#include "oxygine-framework.h"
#include "cUnit.h"

using namespace oxygine;

DECLARE_SMART(cPlayer, spcPlayer);

class cPlayer : public Actor{
	public :
		cPlayer();

		string &getNick(){return nick;};
		void setNick(string n){nick=n;};
		void setID(int i){id = i;}
		int & getID(){return id;};
		spSprite getIcon();

	private:
		spSprite sprite;
		Vector2 pos;
		string nick;
		int id;
};

