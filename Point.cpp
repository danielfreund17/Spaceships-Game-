#include "point.h"
#include "TheShipsGame.h"

int Point::move(int dir)
{//The function recives an integer direction
//the function moves the ship to the relevant direction by changing it's x and y values.
		switch (dir) {
	case 1: // DOWN
		y++;
		if (y > 23) {
			y=0;
			return 1;
		}
		break;
	case 2: // LEFT
		x--;
		if (x < 0) {
			x=79;
			return 1;
		}
		break;
	case 3: // RIGHT
		x++;
		if (x > 79)
		{
			x = 0;
			return 1;
		}
		break;
	case 4:  // UP
		y--;
		if (y < 0) {
			y=23;
			return 1;
		}
		break;
	}
}
void Point::draw(char ch,TheShipsGame *g)
{//the function recives a char , a size, and a pointer to the game
//The function prints the spaceship and updates the game board.
	gotoxy(x, y);
	cout << ch << endl;
	if(g!=nullptr)
		g->printCH(y, x, ch);
}

Point Point::next(int direction)
{//The function recives a direction, the function returns the next position of the ship due to the direction was given.
	Point next = *this;
	next.move(direction);
	return next;
}