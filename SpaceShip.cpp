#define _CRT_SECURE_NO_WARNINGS
#include "SpaceShip.h"
#include "TheShipsGame.h"
class GameManager;
void SpaceShip::move(int size, TheShipsGame *g, Item *item)//item is default memer!
{//The function recives a ship size, a point to the game, and a pointer to an item which is default character
//The function moves the ship to the relevant direction.
	setTextColor(color);
	if (this->flagItemOnTop)
		EnterHoleWithItem(size, g, item);
	drawShip(' ',g);// 
	moveShip();
	drawShip(ch, g);
	setTextColor(Color::WHITE);
}
bool SpaceShip::checkSpaceShip(char board[24][81], int size)
{
	bool res = true;
	if (size == 3)//Small
		res = checkSmall(board, size);
	else
		res = checkBig(board, size);
	return res;
}
bool SpaceShip::checkBig(char board[24][81], int size)
{
	bool res = true;
	if (numOfPoints != size)
		return false;
	else
	{
		int x = getBestPoint().getX();
		int y = getBestPoint().getY();
		//##
		//##
		//##
		if (numOfCols == 2 && numOfRows == 3) {
			pos = 's';//stand
			if (ch != board[(y + 1) % 24][x % 80])
				return false;
			else if (ch != board[(y + 2) % 24][x % 80])
				return false;
			else if (ch != board[y % 24][(x+1) % 80])
				return false;
			else if (ch != board[(y + 1) % 24][(x+1) % 80])
				return false;
			else if (ch != board[(y + 2) % 24][(x+1) % 80])
				return false;
		}
		else {
			//###
			//###
			pos = 'l';//lay
			if (ch != board[y % 24][(x + 1) % 80])
				return false;
			else if (ch != board[y % 24][(x + 2) % 80])
				return false;
			else if (ch != board[(y+1) % 24][x % 80])
				return false;
			else if (ch != board[(y + 1) % 24][(x + 1) % 80])
				return false;
			else if (ch != board[(y + 1) % 24][(x + 2) % 80])
				return false;
		}
	}
	return res;
}
bool SpaceShip::checkSmall(char board[24][81], int size)
{
	bool res = true;
	if (numOfPoints != size)
		return false;
	else
	{
		int x = getBestPoint().getX();
		int y = getBestPoint().getY();
		//# 
		//# 
		//# 
		int tempY;
		int tempX;
		if (numOfCols==1 && numOfRows==3) {
			pos = 's';
			if (ch != board[(y + 1) % 24][x % 80])
				return false;
			else if (ch != board[(y +2) % 24][x % 80])
				return false;
		}
		else {
			//###
			pos = 'l';
			if (ch != board[y%24][(x+1)%80])
				return false;
			else if (ch != board[y % 24][(x + 2) % 80])
				return false;
		}
	}
	return res;
}
Point SpaceShip::getBestPoint()
{
	Point best;
	int index = 0;
	bool flagX = false, flagY = false;
	if (body[0].getX() == 0 && numOfParts == 2)
	{
		flagX = true;
		direction = 3;
		moveShip();
		moveShip();
		moveShip();
	}
	if (body[0].getY() == 0 && numOfParts == 2)
	{
		flagY = true;
		direction = 1;
		moveShip();
		moveShip();
		moveShip();
	}
	best.setX_Y(body[0].getX(), body[0].getY());
	for (unsigned int i = 1; i < numOfPoints; i++)
	{
		if (body[i].getX()<best.getX() || body[i].getY()< best.getY())
		{
			best.setX_Y(body[i].getX(), body[i].getY());
			index = i;
		}
	}
	if (flagX)
	{
		direction = 2;
		moveShip();
		moveShip();
		moveShip();
		direction = 0;
	}
	if (flagY)
	{
		direction = 4;
		moveShip();
		moveShip();
		moveShip();
		direction = 0;
	}
	best.setX_Y(body[index].getX(), body[index].getY());
	return best;
}
void SpaceShip::drawShip(char ch, TheShipsGame *g)
{
	for (unsigned int i = 0; i < numOfPoints;i++)
		body[i].draw(ch, g);
}

int SpaceShip::moveShip()
{

	int res = 0;
	if (direction == 5 && flagItemOnTop==false)//big ship
	{
		int x = getBestPoint().getX();
		int y = getBestPoint().getY();
		body[0].setX_Y(x, y);
		if (numOfPoints == 6)
		{
			if (pos == 'l' && (y == 0 || y == 1))
				return res;
			if (pos == 'l') {//laying
					body[1].setX_Y(body[0].getX() + 1, body[0].getY() - 1);
					body[2].setX_Y(body[0].getX(), body[0].getY());
					body[3].setX_Y(body[0].getX() + 1, body[0].getY());
					body[4].setX_Y(body[0].getX(), body[0].getY() + 1);
					body[5].setX_Y(body[0].getX() + 1, body[0].getY() + 1);
					body[0].setX_Y(body[0].getX(), body[0].getY() - 1);
					pos = 's';
			}
			else
			{//standing
					body[1].setX_Y(body[0].getX() + 1, body[0].getY() + 1);
					body[2].setX_Y(body[0].getX() + 2, body[0].getY() + 1);
					body[3].setX_Y(body[0].getX(), body[0].getY() + 2);
					body[4].setX_Y(body[0].getX() + 1, body[0].getY() + 2);
					body[5].setX_Y(body[0].getX() + 2, body[0].getY() + 2);
					body[0].setX_Y(body[0].getX(), body[0].getY() + 1);

					pos = 'l';

			}
		}
		else//small ship
		{
			if (pos == 'l' && ( y == 0 || y == 1))
				return res;
			if (pos == 'l')
			{
					body[1].setX_Y(body[0].getX(), body[0].getY() - 1);
					body[2].setX_Y(body[0].getX(), body[0].getY());
					body[0].setX_Y(body[0].getX(), body[0].getY() - 2);
					pos = 's';
			}
			else//standing
			{
					body[1].setX_Y(body[0].getX() + 1, body[0].getY() + 2);
					body[2].setX_Y(body[0].getX() + 2, body[0].getY() + 2);
					body[0].setX_Y(body[0].getX(), body[0].getY() + 2);
					pos = 'l';
			}

		}
		//buttom++;
	}
	else
	{
			for (unsigned int i = 0; i < numOfPoints; i++) {
				if (body[i].move(direction) == 1)
					res = 1;
			}
	}
	return res;

}
bool SpaceShip::checkTilt(char board[24][81])
{
	bool tilt = false;
	int prev_dir = getDirection();
	SpaceShip next;
	int x, y;
	if (pos == 'l')//laying
		setDirection(Direction::UP);//up
	else//standing
		setDirection(Direction::RIGHT);//right
	for (unsigned int j = 0; j < 2; j++)
	{
		next = this->NextPosition();
		for (unsigned int i = 0; i < numOfPoints && tilt==false; i++)
		{
			Point next_p = next.body[i];

			x = next_p.getX();
			y = next_p.getY();

			if (numOfPoints == 3) {
				if (board[y][x] == '+' || board[y][x] == '#')
					tilt = true;
			}
			else
			{
				if (board[y][x] == '+' || board[y][x] == '@')
					tilt = true;
			}
			if (board[y][x] == '*')//the item
				tilt = true;// return 3;
		}
	}
	setDirection(prev_dir);
	return tilt;
}
void SpaceShip::getNumber(const Point& point, char theNumber[9], char board[24][81])
{
	Point temp;
	int x = point.getX();
	int y = point.getY();
	bool flag = false;
	if ((board[y][x] >= '0') && (board[y][x] <= '9'))
	{
		flag = true;
		if (direction == 2)
			for (size_t i = 0; i < 5 && (x - i) >= 0 && flag; i++)
			{
				theNumber[8 - i] = board[y][x - i];
				if (((board[y][x - i] >= '0') && (board[y][x - i] <= '9')))
				{
					temp.setX_Y(x - i, y);
					temp.draw(' ', nullptr);
					board[y][x - i] = ' ';
				}
				else
					flag = false;

			}
		else if (direction == 3)
			for (int i = 0; i < 5 && x + i < COLS && flag; i++)
			{
				theNumber[i] = board[y][x + i];
				if (((board[y][x + i] >= '0') && (board[y][x + i] <= '9')))
				{
					temp.setX_Y(x + i, y);
					temp.draw(' ', nullptr);
					board[y][x + i] = ' ';
				}
				else
					flag = false;
			}
		else if (direction == 4 || direction == 1)
		{
			//left
			int lastP = 4;
			for (int i = 0; i < 5 && (x - i) >= 0 && flag; i++)
			{
				theNumber[4 - i] = board[y][x - i];
				if (((board[y][x - i] >= '0') && (board[y][x - i] <= '9')))
				{
					temp.setX_Y(x - i, y);
					temp.draw(' ', nullptr);
					board[y][x - i] = ' ';
				}
				else
				{
					flag = false;
				}

			}
			flag = true;
			//right
			for (int i = 1; i < 5 && (x + i) < COLS && flag; i++)
			{
				lastP++;
				theNumber[lastP] = board[y][x + i];
				if (((board[y][x + i] >= '0') && (board[y][x + i] <= '9')))
				{
					temp.setX_Y(x + i, y);
					temp.draw(' ', nullptr);
					board[y][x + i] = ' ';
				}
				else
					flag = false;
			}
		}
	}
	else
		theNumber[0] = 'N';
}
int SpaceShip::isWall(char board[24][81])
{
	bool wall = false, item = false, badTilt = false, num=false;
	int x, y;
	SpaceShip next = this->NextPosition();
	if (direction == 5)//tilt
		badTilt = checkTilt(board);
	for (unsigned int i = 0; i < numOfPoints; i++)
	{
		Point next_p = next.body[i];

		x = next_p.getX();
		y = next_p.getY();

		if (numOfPoints == 3) {
			if (board[y][x] == '+' || board[y][x] == '#')
				wall = true;//return 1;
		}
		else
		{
			if (board[y][x] == '+' || board[y][x] == '@')
				wall = true;//return 1;
		}
		if (board[y][x] == '*')//the item
			item = true;// return 3;
		else if (board[y][x] >= '0' && board[y][x] <= '9') {
			Point p;
			p.setX_Y(x, y);
			char theNumber[9] = "        ";
			getNumber(p, theNumber, board);
			char* x = strtok(theNumber, " ");
			tokenNumber = atoi(x);
			num = true;
		}
		else if (board[y][x] == '!' || board[y][x] == '$')
			return 7;
	}
	if (wall == true && item == true)//both item and wall.
		return 5;
	else if (wall == true || badTilt==true)
		return 1;
	else if (item == true)
		return 3;
	else if (num == true)
		return 6;
	else
		return false;
}
void SpaceShip::insertPoint(int x, int y)
{
	body[numOfPoints].setX_Y(y, x);
	numOfPoints++;
}
bool SpaceShip::checkIfExist(int x, int y)
{
	bool res = false;
	for (unsigned int i = 0; i < numOfPoints; i++)
	{
		if (body[i].getX() == y && body[i].getY() == x) {
			res = true;
			break;
		}
	}
	return res;
}
void SpaceShip::EnterHoleWithItem(int size, TheShipsGame *g, Item *item )
{//The function recives a size, a pointer to the game, and a pointer to a item
//The function checks if there's a hole coming in the screen, and updates item->OnTopHole to be true
//only if the item is "sitting" on the ship while entering to the hole.
	int Hole = 0;
	SpaceShip nextShip = this->NextPosition();
	if (direction == 4)//up
		if (item != nullptr)
			for (unsigned int i = 0; i < item->getSize(); ++i)//if going up- item moves first forward to the hole, checking when we reach it.
				nextShip = nextShip.NextPosition();
	Hole = nextShip.moveShip();
	if (item != nullptr)
		if (Hole == 1 && item->flagItemOnTop && this->flagItemOnTop)
			item->OnTopHole = true;
}
SpaceShip SpaceShip::NextPosition()
{//The function recives nothing
//The function get's the next point (moving one step forward to the current direction that set)
//the function returns the value of the next point.
	SpaceShip next_p = *this;
	next_p.moveShip();
	return next_p;// next_p.move(direction);
}

bool SpaceShip::CheckItem(Item &item)
{
	int item_x, item_y;
	int x, y;
	Point i_item;
	for (unsigned int i = 0; i < this->getSize(); ++i)
	{
		x = this->body[i].getX();
		y = this->body[i].getY();
		for (unsigned int j = 0; j < item.getSize(); ++j)
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
int SpaceShip::getOppositeDir()
{//the function returns the opposit diretion of the ship
	if (direction == 1)
		return 4;
	else if (direction == 2)
		return 3;
	else if (direction == 3)
		return 2;
	else if (direction == 4)
		return 1;
	else
		return 0;
}
void SpaceShip::drawExplosion()
{//the function draws explosion when the ship is being killed by the item.
	SpaceShip ship = *this;
	int x, y;
	Point p;
	setTextColor(Color::RED);
	MakeBeep(400, 50);
		for (unsigned int i = 0; i < numOfPoints;++i)
		{
			p = ship.body[i];
			x = p.getX();
			y = p.getY();
			gotoxy(x, y);
			cout << ' ' << endl;
			Sleep(20);
		}
		MakeBeep(400, 50);
		for (unsigned int i = 0; i < numOfPoints; ++i)
		{
			p = ship.body[i];
			x = p.getX();
			y = p.getY();
			gotoxy(x, y);
			cout << ch << endl;
			Sleep(20);
		}
		MakeBeep(400, 50);
		for (unsigned int i = 0; i < numOfPoints; ++i)
		{
			p = ship.body[i];
			x = p.getX();
			y = p.getY();
			gotoxy(x, y);
			cout << ' ' << endl;
			Sleep(20);
		}
	setTextColor(Color::WHITE);
}

void SpaceShip::init()
{
	numOfRows = 1;
	numOfCols = 1;
	numOfPoints = 0;
	direction = 0;
	buttom = 0;
	numOfParts = 0;
	for (unsigned int i = 0; i < 6; ++i)
		body[i].init();
}