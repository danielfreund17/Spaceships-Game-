#ifndef _POINT_H_
#define _POINT_H_

#include <iostream>
#define max(x,y) x>y ? x:y
#define min(x,y) x>y ? y:x

using namespace std;
class TheShipsGame;
class Point {
	int x=0, y =0, tilt =0;
	int smallX, bigX, smallY, bigY;
public:

	Point(int _x = 0, int _y = 0) : x(_x), y(_y) {};
	void draw(char ch, TheShipsGame *g);
	int move(int direction);
	Point next(int direction);
	int getX()const { return x; };
	int getY()const { return y; };
	void setX_Y(int _x, int _y){ x = _x; y = _y; }
	void init(){ x = y = tilt = 0; }
};
#endif