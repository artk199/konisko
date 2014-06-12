#pragma once
#include <string>

class cSerializable
{
public:
	virtual void deserialize(std::string s){};
protected:
	cSerializable(void);
	~cSerializable(void);
};

