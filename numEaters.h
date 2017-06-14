#ifndef numEaters_h
#define numEaters_h
#include "item.h"

class NumEaters: public Item{
	int xTarget, yTarget;

public:
	bool ChasingNum = false;
	void set() { Item::setCH('%'); }
	int getxTarget()const{ return xTarget; }
	int getyTarget()const{ return yTarget; }
	int getxPos(){ return Item::getXval(); }
	int getyPos(){ return Item::getYval(); }
	void insertTargets(int _x, int _y){ xTarget = _x; yTarget = _y; }
	void insertPos(int _x, int _y);
};

#endif