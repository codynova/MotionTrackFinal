#pragma once
#include <string>

using namespace std;

class Thing{

public:
	Thing();
	~Thing();

	int getXPos();
	void setXPos(int x);

	int getYPos();
	void setYPos(int y);


private:
	int xPos, yPos;
	string type;
};

