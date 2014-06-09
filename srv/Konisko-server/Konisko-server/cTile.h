#pragma once
class cTile
{
public:
	cTile(void);
	cTile(int type);
	~cTile(void);
	bool isMoveable();
private:
	bool moveable;
};

