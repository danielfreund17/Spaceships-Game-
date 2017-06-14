#ifndef _Ship_H_
#define _Ship_H_
#include "Color.h"
#include "point.h"
#define Small 0
#define Big 1
class Item;
class TheShipsGame;


class SpaceShip{
	Point body[6];
	Color color;
	int ShipSize;
	int numOfPoints = 0;
	int direction =0;
	int numOfParts = 0;
	int numOfRows;
	int numOfCols;
	int tokenNumber;
	char pos;
	
	char ch = '*';
	int buttom=0;
public:
	bool flagItemOnTop = false;
public:
	SpaceShip() {};
	Point getPosition() { return body[6]; };
	int getTokenNumber() { return tokenNumber; };
	void setColor(Color c) { color = c; };
	void set(char c) { ch = c; };
	void move(int size, TheShipsGame *g=nullptr,Item *item=nullptr);//prototype; item is default member!
	void setDirection(int _direction) { direction = _direction; };//Inline
	SpaceShip NextPosition();
	void addRows() { numOfRows++; };
	void addCols() { numOfCols++; };
	Point getBestPoint();
	int getDirection()const {return direction;}
	int getSize()const { return numOfPoints; }
	int getOppositeDir();
	void drawExplosion();
	void getNumber(const Point& point, char theNumber[9], char board[24][81]);
	void addParts() { numOfParts++; };
	void EnterHoleWithItem(int size, TheShipsGame *g, Item *item);
	void insertPoint(int x, int y);
	bool checkIfExist(int x, int y);
	int isWall(char board[24][81]);
	void drawShip(char ch, TheShipsGame *g);
	int moveShip();
	bool checkTilt(char board[24][81]);
	bool CheckItem(Item &item);
	bool checkSpaceShip(char board[24][81], int size);
	bool checkSmall(char board[24][81], int size);
	bool checkBig(char board[24][81], int size);
	void init();
};
#endif