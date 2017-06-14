#include "item.h"
#include "TheShipsGame.h"
#include "point.h"
class TheShipsGame;

void Item::updatePosition(int x, int y)
{
	body.clear();
	insertPoint(y, x);
}

void Item::move(TheShipsGame *g)
{//The function recives a pointer to the game.
//The function moves the item to the relevant direction
	draw(' ', g);
	moveItem(direction);
	draw(ch,g);
}
void Item::moveItem(int direction)
{//The function recives an integer direction
 //the function moves the ship to the relevant direction by changing it's x and y values.
	Point temp;
	for (list<Point>::iterator itr = body.begin(); itr != body.end(); ++itr)
	{
		//body[i].move(direction);aa
		itr->move(direction);
	}
}
void Item::insertPoint(int x, int y)
{	
	Point temp;
	temp.setX_Y(y, x);
	body.push_back(temp);
	//body[numOfPoint].setX_Y(y, x);
	numOfPoint++;
}
Point Item::NextPosition()
{//The function returns the next point of the current place of the item.
	Point next_p;// = this->body;
	return next_p.next(direction);
}
int Item::isWall(char board[24][81])
{//The function recives a reference to the body of the item, and the game board
//the function returns values due the chars that the item is tackeling while moving.

	int x, y;
	for (list<Point>::iterator itr = body.begin(); itr != body.end(); ++itr)
	{
		Point next_p = *itr;

		x = next_p.next(direction).getX();
		y = next_p.next(direction).getY();
		if (board[y][x] == '+')//wall
			return 1;
		else if (board[y][x] == '#')//big ship
			return 2;
		else if (board[y][x] == '@')//small ship
			return 3;
		else if (board[y][x] == '*' && notMe(y, x))
			return 4;
		else if (board[y][x] == '!' || board[y][x] == '$')
			return 5;	
	}
	return false;
}
bool Item::notMe(int y, int x)
{
	int xME, yME;
	for (list<Point>::iterator itr = body.begin(); itr != body.end();itr++)
	{
		xME = itr->getX();
		yME = itr->getY();
		if (yME == y && xME == x)
			return false;
	}
	return true;
}
bool Item::CheckItem(Item &item)
{
	int item_x, item_y;
	int x, y;
	int direc = getDirection();
	Point j_item;
	Point i_item;
	for (unsigned int i = 0; i < this->body.size(); ++i)
	{
		j_item = this->get_i_point(i);
		j_item=j_item.next(direc);
		x = j_item.getX();
		y = j_item.getY();
		for (unsigned int j = 0; j < item.body.size(); ++j)
		{
			i_item = item.get_i_point(j);
			item_x = i_item.getX();
			item_y = i_item.getY();
			if (x == item_x && y == item_y)
				return true;
		}
	}
	return false;
}
bool Item::DeleteItem()
{
	return body.empty();
}
void Item::ConnectItems(Item &item)
{
	int x, y;
	Point p;
	int sizeT = item.getSize();
	for (unsigned int i = 0; i < sizeT; i++)
	{
		p = item.body.back();
		x = p.getX();
		y = p.getY();
		insertPoint(y, x);
		item.body.pop_back();
		item.numOfPoint--;
	}
}
void Item::init()
{
	numOfPoint = 0;
	size = 2;
	direction = 1;
	for (list<Point>::iterator itr = body.begin(); itr != body.end(); ++itr)
	{
		itr->init();
	}
	body.clear();
}
const Point & Item::get_i_point(int i) 
{
	int place = 0;
	list<Point>::iterator itr=body.begin();
	for (itr = body.begin(); place < i; ++itr, place++);

	return *itr;
}
void Item::draw(char ch, TheShipsGame* g)
{//The function recives a char and a pointer to the game
//The function prints the item in the relevant points of the board and screen, and updates the game board.
	int x, y;
	setTextColor(LIGHTMAGENTA);
	for (list<Point>::iterator itr = body.begin(); itr != body.end(); ++itr)
	{
		x = itr->getX();
		y = itr->getY();
		setTextColor(color);

		gotoxy(x, y);
		cout << ch << endl;
		g->printCH(y, x, ch);//updating game board
	}
	setTextColor(Color::WHITE);
}

int Item::getXval()
{
	list<Point>::iterator itr = body.begin();
	return itr->getX();
}
int Item::getYval()
{
	list<Point>::iterator itr = body.begin();
	return itr->getY();
}

