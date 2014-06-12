#pragma once
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
private:
	bool walkable;
	bool destroyable;
};

