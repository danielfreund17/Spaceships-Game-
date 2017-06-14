Name: Daniel Freund
ID: 304841042
Name: Idan Dagan
ID: 303012314

Important!!!- using Visual Studio 2013

Both students belongs to class B.


notes:
1)We setted the direction of the "line flyers" to move left if it's '!', and to move up if it's '$'.
it can be changed easily, as needed.
The function that sets the direction is function "set()" in Flyers.cpp:

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

2)in the previous exercivse (number 2), we got minus 2 points because of "using numbers and strings without const set".
we don't see where we use such a thing, we use enums (we have class Direction for that), and we really checked and didn't find where we use numbers\strings,
instead of enums\consts.

3) In our algorithm, the "Numbers Eater" moves twice faster as the ships, as needed.
we use in the algorithm 2 moves each iteration, as the ships uses one move each iteration.
as we built this algorithm, it may be possible that the eater moves for example UP and then, in the same iteration, at the second move, RIGHT.
and it might seem like it moves slant, but it doesn't- the eater moves only UP DOWN RIGHT or LEFT.