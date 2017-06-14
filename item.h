#ifndef _ITEM_H_
#define _ITEM_H_
#include "Color.h"
#include "point.h"
#include <list>

class GameManager;
class Item {
	list<Point> body;
	Color color = LIGHTMAGENTA;

	int numOfPoint = 0;
	int size = 2;
	int direction=1;
	char ch = '*';
public:
	bool flagItemOnTop = false;
	bool OnTopHole = false;
public:
	bool notMe(int y, int x);
	bool CheckItem(Item &item);
	void ConnectItems(Item &item);
	bool DeleteItem();
	void setColor(Color c) { color = c; };
	void set(char c) { ch = c; };
	void setSize(int _size) { size = _size; };
	void move(TheShipsGame *g);//prototype;
	void moveItem(int direction);
	Point NextPosition();
	int isWall(/*Point body[], */char board[24][81]);
	void setDirection(int dir) { direction = dir; };
	int getDirection() { return direction; };
	int getSize() { return numOfPoint; };
	void draw(char ch, TheShipsGame* g);
	void insertPoint(int x, int y);
	const Point & get_i_point(int i);
	bool isItemValid();
	void setCH(char _ch){ ch = _ch; }
	char getCH(){ return ch; }
	virtual void init();
	void updatePosition(int x, int y);
	int getXval();
	int getYval();
};
#endif