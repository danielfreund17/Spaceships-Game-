#include "Flyers.h"
#include "Direction.h"
#include "Color.h"

void Flyers:: set() {
	if (Item::getCH() == '$')
	{
		Item::setDirection(Direction::UP);
		Item::setColor(LIGHTCYAN);
	}
	else
	{
		Item::setDirection(Direction::LEFT);
		Item::setColor(LIGHTRED);
	}
}