#pragma once

class cGame;
class cTile
{
public:
	cTile(void);
	cTile(int type);
	~cTile(void);
	void setWalkable(bool s); 
	void setDestroyable(bool s);
	bool isWalkable();
	bool isDestroyable();
	void setPos(int x, int y);

	bool destroy(cGame *g); // zwraca true, jezeli ogien ma isc dalej
private:
	bool walkable;
	bool destroyable;
	int pozx,pozy;
};

