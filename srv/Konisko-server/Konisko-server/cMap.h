#pragma once
#include <string>

class cMap
{
public:
	cMap(void);
	cMap(int type);
	~cMap(void);
	bool canMove(std::pair<double,double> pos);
	void update(double delta);
};

