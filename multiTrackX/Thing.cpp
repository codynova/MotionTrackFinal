#include "Thing.h"


Thing::Thing(){}


Thing::~Thing(){}


int Thing::getXPos(){
	return Thing::xPos;
}


void Thing::setXPos(int x){
	Thing::xPos = x;
}


int Thing::getYPos(){
	return Thing::yPos;
}


void Thing::setYPos(int y){
	Thing::yPos = y;
}