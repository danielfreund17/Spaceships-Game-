#ifndef POINT_NUM
#define POINT_NUM

class pointNum{
	int x;
	int y;
public:
	pointNum(int _x, int _y) : x(_x), y(_y){}
	int getX()const{ return x; }
	int getY()const{ return y; }
};

#endif