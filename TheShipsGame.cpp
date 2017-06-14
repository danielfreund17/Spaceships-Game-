#define _CRT_SECURE_NO_WARNINGS
#include "TheShipsGame.h"
#include "Keyboard.h"
#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <sstream>
#include <string>

bool TheShipsGame::isScreenIvalid(int num)
{
	return screens[num]->isValid;
}
void TheShipsGame::FreeSaveScreen()
{
	//All other allocation already being deleted in the main free screens because of operator =
	saveScreen->FreeID();
	saveScreen->FreeBoard();
	delete saveScreen;
}
void TheShipsGame::FreeScreens()
{
	for (unsigned int i = 0; i < numLevels; ++i)
	{
		screens[i]->FreeID();
		screens[i]->FreeBoard();
		delete screens[i];
	}
	delete[]screens;
}
bool TheShipsGame::isLevelDone()
{
	 return levelDone; 
}
void TheShipsGame::LevelIsDone()
{
	if (!nowRunningSol)
	{
		screens[playingLevel]->Finished();
		playingLevel++;
		Life = 3;
	}
}
int stringToInt(const string& string)
{
	int strSize = string.size()-16;
	char* str;
	size_t i;
	str = new char[strSize + 1];
	for (i = 16; i < strSize+16; i++)
		str[i] = string[i];
	str[i] = '\0';
	int ID;// = new int;
	ID = atoi(str+16);//getting id converting to int
	return ID;
}
int strToInt(const string& string)
{
	int strSize = string.size()+1;
	char* str;
	size_t i;
	str = new char[strSize];
	for (i = 0; i < strSize-1 ; i++)
		str[i] = string[i];
	str[i] = '\0';
	int ID;// = new int;
	ID = atoi(str);//getting id converting to int
	return ID;
}
int TheShipsGame::getDirection(int shipIndex, const list<char>& keyHits)
{//The function recives a ship index, and a list (const by ref)
//the function knows to distinguish between the small ship and the big one, and returns the needed direction by the key was pressed.
	static const char* keys[] = { "mjlink", "xadwzs" };
	for (char key : keyHits)
	{
		for (int i = 0; i < 6; ++i) {
			if (key == keys[shipIndex][i]) {
				return i + 1;
			}
		}
	}
	return -1;
}
int TheShipsGame::getNewDirection(int shipIndex, string& str)
{//The function recives a ship index, and a list (const by ref)
 //the function knows to distinguish between the small ship and the big one, and returns the needed direction by the key was pressed.
	static const char* keys[] = { "mjlink", "xadwzs" };
	for (char key : str)
	{
		for (int i = 0; i < 6; ++i) {
			if (key == keys[shipIndex][i]) {
				return i + 1;
			}
		}
	}
	return -1;
}
char TheShipsGame::convertDirection(const list<char>& keyHits,int index)
{//The function recives a ship index, and a list (const by ref)
 //the function knows to distinguish between the small ship and the big one, and returns the needed direction by the key was pressed.
	static const char* keys[] = { "mjlink","xadwzs" };
	for (char key : keyHits)
	{
		for (int i = 0; i < 6; ++i) {
			if (key == keys[index][i]) {
				return key;
			}
		}
	}
	return -1;
}
Item& TheShipsGame::get_item_i(int i)
{
	int place = 0;
	list<Item>::iterator itr = item.begin();
	for (itr = item.begin(); place < i; ++itr, place++);

	return *itr;
}
Flyers& TheShipsGame:: get_flyer_i(int i)
{
	int place = 0;
	list<Flyers>::iterator itr;
	for (itr = FlyList.begin(); place < i; ++itr, place++);

	return *itr;
}
NumEaters& TheShipsGame:: get_eater_i(int i)
{
	int place = 0;
	list<NumEaters>::iterator itr;
	for (itr = EatNums.begin(); itr != EatNums.end() && place < i; ++itr, place++);
	
	return *itr;
}

void TheShipsGame::WhatToDoFlyer(int obstacle, int size)
{
	Flyers *me;
	int numitemTackled;
	switch (obstacle)
	{
	case 2:
		ship[Big].drawExplosion();
		screens[playingLevel]->eraseMathEquition();
		screens[playingLevel]->convertParameters();
		clear_screen();
		Life--;
		PlayingSavedGame = false;
		init();
		break;
	case 3:
		ship[Small].drawExplosion();
		screens[playingLevel]->eraseMathEquition();
		screens[playingLevel]->convertParameters();
		clear_screen();
		Life--;
		PlayingSavedGame = false;
		init();
	case 4:
		me = &get_flyer_i(size);
		numitemTackled = WhichItem(ship[Small], me, size);
		get_item_i(numitemTackled).draw(' ', this);
		get_item_i(numitemTackled).init();
		DeleteDeadItems();
	default:
		get_flyer_i(size).move(this);
	}
}
void TheShipsGame::WhatToDoEater(int obstacle, int size)
{
	NumEaters &eater = get_eater_i(size);//reference to numEater
	int xPos, yPos;
	xPos = eater.getxPos();
	yPos = eater.getyPos();
	if (obstacle == false)//didn't hit anything or hited a number
	{
		if (eater.getyTarget() == -1)
			return;
		else
		{
			///move function we have to build
			if (xPos < eater.getxTarget())
			{
				eater.setDirection(Direction::RIGHT);
				xPos++;
			}
			else if (xPos>eater.getxTarget())
			{
				eater.setDirection(Direction::LEFT);
				xPos--;
			}
			obstacle = eater.isWall(board);
			if (obstacle == false)
			{
				eater.move(this);
				xPos = eater.getxPos();
				yPos = eater.getyPos();
				eater.insertPos(xPos, yPos);
			}
			else
			{
				eater.draw(' ', this);
				eater.init();
				DeleteDeadEatnums();
				return;
			}
			if (xPos == eater.getxTarget() && yPos == eater.getyTarget())
			{
				DeleteNum(xPos, yPos);
				FindMinDistance(eater);
			}
			if (yPos < eater.getyTarget())
			{
				eater.setDirection(Direction::DOWN);
				yPos++;
			}
			else if (yPos>eater.getyTarget())
			{
				eater.setDirection(Direction::UP);
				yPos--;
			}
			obstacle = eater.isWall(board);
			if (obstacle != false)
			{
				eater.draw(' ', this);
				eater.init();
				DeleteDeadEatnums();
				return;
			}
			eater.move(this);
			xPos = eater.getxPos();
			yPos = eater.getyPos();
			eater.insertPos(xPos, yPos);
			if (xPos == eater.getxTarget() && yPos == eater.getyTarget())
			{
				DeleteNum(xPos, yPos);
				FindMinDistance(eater);
			}
		}
	}
	else
	{
		eater.draw(' ', this);
		eater.init();
		DeleteDeadEatnums();
	}
}

void TheShipsGame::DeleteNum(int x, int y)
{
	for (vector<pointNum>::iterator itr = NumberPositions.begin(); itr != NumberPositions.end(); itr++)
	{
		if (itr->getX() == x && itr->getY() == y)
		{
			NumberPositions.erase(itr);
			numOfNumbers--;
			x++;
			while (board[y][x] >= '0' && board[y][x] <= '9')
			{
				board[y][x] = ' ';
				gotoxy(x, y);
				cout << " ";
				x++;
			}
			return;
		}
	}
}

void TheShipsGame::DeleteDeadEatnums()
{
	for (list<NumEaters>::iterator itr = EatNums.begin(); itr != EatNums.end(); ++itr)
	{
		if (itr->DeleteItem())
		{
			EatNums.erase(itr);
			numOfEaters--;
			break;
		}
	}
}
void TheShipsGame::WhatToDo(int obstacle, SpaceShip ship[], int size)
{//The function recvies an obstacle, an array of 2 ships and a size.
	//The function makes an action due to the obstacle recives which was set in isWall function due ship's movement and which char he was tackled.
	int num_item;
	int res;
	int num;
	int whichItemOnTop = 0;
	string numString;
	bool itemOnTop = false;
	bool localItemOnTop = false;
	for (unsigned int i = 0; i < numOfItems; ++i)
	{
		localItemOnTop = IsItemOnTop(ship[size], size, get_item_i(i));
		if (localItemOnTop)
		{
			whichItemOnTop = i;
			itemOnTop = true;
		}
	}
	if (itemOnTop)
		ship[size].flagItemOnTop = true;
	switch (obstacle)
	{
	case 0://There is no wall - so continue.
		if (ship[size].getDirection() == 5 && itemOnTop)
			break;
		MoveWithItem(ship[size], size, get_item_i(whichItemOnTop), itemOnTop);//if item on top is true- move with item, if not- move normally due to conditions
		MoveWithItemToHole(ship[size], size, get_item_i(whichItemOnTop));//if item on top is true and moving through a hole
		break;
	case 3://Spacecraft hit an item
		num_item = WhichItem(ship[size]);
		if (num_item != -1)
		{
			SpacePushItem(ship, size, get_item_i(num_item));
			MoveWithItemToHole(ship[size], size, get_item_i(num_item));
		}
		break;
	case 6://SpaceShip had picked an number.
		num = ship[size].getTokenNumber();
		res = screens[playingLevel]->insertNum(num);
		if (res == 2)
		{
			numString = convertToString(num);
			screens[playingLevel]->pushNumber(numString);
		}
		if (res == 1)
		{
			int bestTime = checkBestTime(screens[playingLevel]->getScreenId());//The function returns the best time for the specific level.
			if ((totalTime < bestTime || bestTime == 0) && nowRunningSol == false)
			{
				saveBestGame();
				levelDone = true;
				if (playingLevel == numLevels - 1)
					FinishedScreens = true;
				keysEnter.clear();
			}
			else {
				keysEnter.clear();
				if (playingLevel == numLevels - 1 && nowRunningSol == false)
					FinishedScreens = true;
				levelDone = true;
			}
		}
		else if (res == 0)
			TookWrongAnswer(ship, size);
		break;
	case 7:
		ship[size].drawExplosion();
		screens[playingLevel]->eraseMathEquition();
		screens[playingLevel]->convertParameters();
		clear_screen();
		Life--;
		PlayingSavedGame = false;
		init();
		break;
	default://There is a wall.
		break;
	}
}
void TheShipsGame::DeleteDeadItems()
{
	for (list<Item>::iterator itr = item.begin(); itr != item.end(); ++itr)
	{
		if (itr->DeleteItem())
		{
			item.erase(itr);
			numOfItems--;
			break;
		}
	}
}

void TheShipsGame::WhatToDoItem(int num,int size)
{//The function recives a number, and size (right now size is not being used because there is only one item)
//The function do actions depens to num, which was set in item.isWall function due to next point of the item while moving.
	int obs;
	Item tackledItem;
	Item *me;
	NumEaters *eater;
	int j = 0;
	int numitemTackled;
	switch (num)
	{
	case 1://Wall
		get_item_i(size).setDirection(Direction::STAY);
		break;
	case 2://Big SpaceShip
		if (get_item_i(size).getDirection() == 1 && get_item_i(size).flagItemOnTop == false && ship[Big].flagItemOnTop == false){//item is falling, and not sitting on a spaceship.
			ship[Big].drawExplosion();
			screens[playingLevel]->eraseMathEquition();
			screens[playingLevel]->convertParameters();
			clear_screen();
			Life--;
			PlayingSavedGame = false;
			init();
		}
		else
		{
			get_item_i(size).setDirection(ship[Big].getDirection());//sitting on a spaceship or being pushed by one
		}
		break;
	case 3://Small SpaceShip
		if (get_item_i(size).getDirection() == 1 && get_item_i(size).flagItemOnTop == false && ship[Small].flagItemOnTop==false){////item is falling, and not sitting on a spaceship.
			ship[Small].drawExplosion();
			screens[playingLevel]->eraseMathEquition();
			screens[playingLevel]->convertParameters();
			clear_screen();
			Life--;
			PlayingSavedGame = false;
			init();
		}
		else
		{
			get_item_i(size).setDirection(ship[Small].getDirection());////sitting on a spaceship or being pushed by one
		}
		break;
	case 4:
		me = &get_item_i(size);
		numitemTackled= WhichItem(ship[Small], me,size);
		get_item_i(size).ConnectItems(get_item_i(numitemTackled));
		DeleteDeadItems();
		break;
	case 5:
		me = &get_item_i(size);
		me->draw(' ', this);
		me->init();
		DeleteDeadItems();
		break;
	default:
		if (get_item_i(size).flagItemOnTop == false)//nothing on the item's way
		{
			get_item_i(size).setDirection(Direction::DOWN);
			obs = get_item_i(size).isWall(board);
			if (obs == 0)//nothing on the item's way
				get_item_i(size).move(this);
			else
			{
				get_item_i(size).setDirection(Direction::STAY);
				get_item_i(size).move(this);
			}
		}
	}
}
void TheShipsGame::MoveWithItemToHole(SpaceShip &ship, int size, Item &item)
{//The function recives a reference to a ship, it's size, and a reference to an item.
//The function knows to recognize holes in the screen game, and if there's an item on the spaceship while moving to a hole-
//the ship and the item goes out from the other side together.
	int itemDir = item.getDirection();
	int itemSize = item.getSize();
	if (item.OnTopHole && ship.flagItemOnTop && ship.isWall(board) != 1)
	{
		if (itemDir == 2 || itemDir == 3)//CASE: going left or going right
		{
			for (unsigned int i = 0; i < (3); ++i)
			{
				if (ship.isWall(board) != 1)
				{
					ship.move(size, this);
					item.move(this);
				}
			}
			if (itemDir == 3)
			{
				item.setDirection(itemDir);
				item.move(this);
			}
		}
		else if (itemDir == 1)//down
		{
			ship.setDirection(Direction::DOWN);
			for (unsigned int i = 0; i < (itemSize)+2; ++i)//down
			{
				if (ship.isWall(board) != 1)
				{
					ship.move(size, this);
					item.move(this);
				}
			}
			//return true;//keep going with the item!
		}
		else if (itemDir == 4)//up
		{
			for (unsigned int i = 0; i < itemSize + 1; ++i)
			{
				if (ship.isWall(board) != 5 && ship.isWall(board) != 1)
				{
					if (item.isWall(board) !=1)
						item.move(this);
						ship.move(size, this);
				}
			}

		}
	}
	item.OnTopHole = false;//PASSED THE HOLE- OnTopHole=false.
}
void TheShipsGame::SpacePushItem(SpaceShip *ship, int size, Item &item)
{//The function recives a pointer to a ship array (2 ships), size to different between them, and a reference to an item
//if we got to here, it means that a spaceship is trying to push an item (eather both of them together or only one).
//the function has to decide wether to let the ship push the item (depens to it's size), and the function also checks if the ships push the item together.
	bool pushTogether = false;
	int prev_dir1, prev_dir2, OtherShip, spaceSize, spaceSizeSaver;
	if (size == Small) {
		spaceSize = smallSize;
		OtherShip = Big;
	}
	else {
		spaceSize = bigSize;
		OtherShip = Small;
	}
	spaceSizeSaver = spaceSize;//size of the current ship
	prev_dir1 = ship[size].getDirection();
	prev_dir2 = ship[OtherShip].getDirection();
	if ((ship[size].isWall(board) == 3) && (ship[OtherShip].isWall(board) == 3))
		if (prev_dir1 == prev_dir2)
			pushTogether = true;//CASE: both pushing the item but the ships don't touch each other.
	ship[size].setDirection(ship[size].getOppositeDir());
	ship[OtherShip].setDirection(ship[OtherShip].getOppositeDir());
	if ((ship[size].isWall(board) == 5) || (ship[size].isWall(board) == 1) && (ship[OtherShip].getDirection() == ship[size].getDirection()))
		pushTogether = true;//CASE: both pushing each other and one touches the item- we chose to move the item.
	ship[size].setDirection(prev_dir1);//getting original direction
	ship[OtherShip].setDirection(prev_dir2); //getting original direction
	spaceSize = smallSize + bigSize;//calculating size of both ships together.
	if ((pushTogether) && item.getSize() <= (double)((2 * spaceSize) / 3) && item.getDirection() != 1)//the two ships pushes the item
	{
		item.setDirection(ship[size].getDirection());
		if (item.isWall(board) == false)
		{
			item.move(this);
			ship[size].move(size, this, &item);
		}
	}
	else//only one of the ships is touching the item- NOT together
	{//spaceSizeSaver is the original size of the ships that doing the function now
		if (item.getSize() <= (double)((2 * spaceSizeSaver) / 3) && item.getDirection() != 1)
		{
			item.setDirection(ship[size].getDirection());
			if (item.isWall(board) == false)
			{
				item.move(this);
				ship[size].move(size, this, &item);
			}
		}
	}
}
void TheShipsGame::MoveWithItem(SpaceShip &ship, int size, Item &item, bool itemOnTop)
{//The function recives a reference to a ship, it's size, a reference to an ite, and a bollean itemOnTop.
//The function checks first if itemOnTop is true- if it does, that means that an item is "sitting" on a spaceship.
//in that case- due to it's size, the function decides if movement is posssible.
	bool onTopMoveDown = false;
	if ((itemOnTop) && (ship.flagItemOnTop) && (!item.OnTopHole))//if item is on top and not going through a hole
	{
		{
			if (item.isWall(board) == false || ship.getDirection() == 1)// not + not # not @
				if (ship.getDirection() != 1)//not going down- item moves first and then the ship
					item.move(this);
				else
					onTopMoveDown = true;//going down- ship moves first and then the item
		}
		if (onTopMoveDown)//needs movement only after the ships move because going down.
		{
			ship.move(size, this, &item);
			item.move(this);
		}
	}	//down if: not with an item and moving down and also: (without item, or not going up, or item not going through a wall)
	//explaination: if all the 3 things: the spaceships has an item, the spaceships goes up and the item reaches a wall- must stop- not moving.
	//also: if the ship doesn't have any item on it,- move freely due to rules of walls and etc
	if (!onTopMoveDown && (!item.flagItemOnTop || ship.getDirection() != 4 || item.isWall(board) != 1) || !ship.flagItemOnTop)
		ship.move(size, this, &item);//move without item or check wall conditions while moving with item
}
void TheShipsGame::TookWrongAnswer(SpaceShip *ship, int size)
{//The functon recvies a pointer to array of 2 ships, and a relevant size.
//if we got here, that means that we took the wrong number. the function updates the relevant ship's life and restart the current level.
	ship[Big].setDirection(Direction::STAY);
	ship[Small].setDirection(Direction::STAY);
	Life--;
	clear_screen();
	printBoard();
	keysEnter.clear();
	//init();
	if (Life > 0)
	{
		ship[Small].move(Small, this);
		ship[Big].move(Big, this);
	}
}

string TheShipsGame::convertToString(int num)
{
	string numString;
	int digit;
	string digitHelper;
	while (num != 0)
	{
		digit = num % 10;
		digitHelper.push_back(digit+'0');
		numString.insert(0, digitHelper);
		digitHelper.clear();
		num /= 10;
	}
	return numString;
}
void TheShipsGame::getTimeVectors(fstream &file, vector <keyPoint> &moves)
{
	int size = 0, sizeOfVectors;
	string line;
	string token;
	string temp = "";
	int clock_itr = 0;
	string keys;
	getline(file, line);
	char *dup = _strdup(line.c_str());
	token = strtok(dup, " ");
	sizeOfVectors = strToInt(token);
	for (size_t i = 0; i < sizeOfVectors; i++)
	{
		getline(file, line);
		char *dup2 = _strdup(line.c_str());
		token = strtok(dup2, ":");//Getting clock iteration
		clock_itr = strToInt(token);

		token = strtok(NULL," ");//Getting keys that has been entered.
		keys.append(token);

		keyPoint newMove(clock_itr);
		string tempKey = keys;
		newMove.insertChar(tempKey);
		moves.push_back(newMove);
		keys.clear();
	}
}
void TheShipsGame::getBestMove()
{
	int clock_itr = 0;
	int dir = 0;
	string iterations;
	int size = 0,sizeOfVectors;
	char* token;
	string line,bestFile = "";
	bestFile.insert(0,screens[playingLevel]->getScreenId());
	bestFile.erase(0, 9);
	size = bestFile.size();
	bestFile.insert(size, ".spd");
	fstream file(bestFile);
	if (!file.is_open())
	{
		clear_screen();
		cout << "There is no solution for this level." << endl;
		Sleep(800);
		system("dir /b *.spd > spdFiles.txt");
		clear_screen();
	}
	else
	{
		vector <keyPoint> moves;
		getline(file, line);
		getline(file, iterations);
		NumIterations = stringToInt(iterations);
		for (size_t i = 0; i < 27; i++)
			getline(file, line);
		getTimeVectors(file, moves);
		clear_screen();
		init();
		file.close();
		show_solution(moves);
	}
}
void TheShipsGame::show_solution(vector<keyPoint>& moves)
{
	int round = 0;
	int indexITR = 0;
	int bigDir = 6;
	int smallDir = 6;
	int nowTime, direction;
	ship[Small].setDirection(Direction::STAY);
	ship[Big].setDirection(Direction::STAY);
	ship[Small].move(Small,this);
	ship[Big].move(Big, this);

	while (round < NumIterations)
	{
		
		if (_kbhit()) {
			char ch = _getch();
			if (ch == ESC) {
				// ESC pressed
				break;
			}
		}
		setTextColor(Color::LIGHTGREEN);
		printDetails(round);

		if (indexITR < moves.size())
		{
			nowTime = moves[indexITR].getClock();
			if (round == nowTime) {
				if ((direction = getNewDirection(0, moves[indexITR].getKey())) != -1) {//Small Space-Ship
					if (direction != Direction::TILT)
					{
						ship[Small].setDirection(direction);
						LastKeyPressedSmall = direction;
					}
					else
					{
						ship[Small].setDirection(direction);
						obstacle = ship[Small].isWall(board);
						WhatToDo(obstacle, ship, Small);
						ship[Small].setDirection(LastKeyPressedSmall);
					}
				}
				if ((direction = getNewDirection(1, moves[indexITR].getKey())) != -1) {//Big space-Ship
					if (direction != Direction::TILT)
					{
						ship[Big].setDirection(direction);
						LastKeyPressedBig = direction;
					}
					else
					{
						ship[Big].setDirection(direction);
						obstacle = ship[Big].isWall(board);
						WhatToDo(obstacle, ship, Big);
						ship[Big].setDirection(LastKeyPressedBig);
					}
				}
				indexITR++;
			}
		}
		for (unsigned int p = 0; p < numOfItems; p++)
		{
			obstacle = get_item_i(p).isWall(board);
			WhatToDoItem(obstacle, p);
		}
		for (unsigned int p = 0; p < numOfFlyers; p++)
		{
			obstacle = get_flyer_i(p).isWall(board);
			WhatToDoFlyer(obstacle, p);
		}
		for (unsigned int p = 0; p < numOfEaters; p++)
		{
			obstacle = get_eater_i(p).isWall(board);
			WhatToDoEater(obstacle, p);
		}
		if (Life > 0) {
			obstacle = ship[Small].isWall(board);
			WhatToDo(obstacle, ship, Small);
			obstacle = ship[Big].isWall(board);
			WhatToDo(obstacle, ship, Big);
		}
		if (levelDone == true)
		{
			NumIterations = 0;
			return;
		}
		Sleep(120);
	}
}
void TheShipsGame::saveBestGame()
{
	string name;
	bestTime = totalTime;
	clear_screen();
	cout << "Congratulation! You broke a record!" << endl;
	cout << "Please enter your name: " << endl;
	getline(cin, name);
	saveGame(name);
	system("dir /b *.spd > spdFiles.txt");
}
int TheShipsGame::checkBestTime(const string& name)
{
	system("dir /b *.spd > spdFiles.txt");
	ifstream victoryFiles("spdFiles.txt");
	string screen_id, result;
	int size, time = 0;
	while (!victoryFiles.eof())
	{
		getline(victoryFiles, screen_id);
		size = screen_id.size() - 4;
		ifstream checkFile(screen_id);
		if (!screen_id.substr(0, size).compare(name.substr(9,name.size())))
		{
			getline(checkFile, result);
			getline(checkFile, result);
			time = stringToInt(result);
			break;
		}
	}
	return time;
}
int TheShipsGame::WhichItem(SpaceShip &ship, Item *item, int itm)
{
	int x, y;
	int x_item, y_item;
	SpaceShip next_point_ship;
	Item next_point_item;
	int num_items = numOfItems;
	if (item == nullptr)
		next_point_ship = ship.NextPosition();
	for (unsigned int i = 0; i < num_items; ++i)
	{
		if (item == nullptr)
		{
			if (next_point_ship.CheckItem(get_item_i(i)))
				return i;
		}
		else
		{
			if (i != itm && item->CheckItem(get_item_i(i)))
				return i;
		}
	}
	return -1;
}
void TheShipsGame::init()
{//The function recives nothing
//The function do initialization to the game board, the item, and the ships.
	ship[Small].init();
	ship[Big].init();
	for (list<Item>::iterator itr = item.begin(); itr != item.end(); ++itr)
		itr->init();
	for (list<Flyers>::iterator itr = FlyList.begin(); itr != FlyList.end(); itr++)
		itr->init();
	for (list<NumEaters>::iterator itr = EatNums.begin(); itr != EatNums.end(); ++itr)
		itr->init();
	item.clear();
	numOfItems = 0;
	FlyList.clear();
	numOfFlyers = 0;
	numOfEaters = 0;
	EatNums.clear();
	numOfNumbers = 0;
	NumberPositions.clear();
	if (!PlayingSavedGame)
		totalTime = 0;
	bestTime = 0;
	game_over = false;
	inValidScreen = false;
	FinishedScreens = false;
	//FinishedScreens = false;
	maxRowOfSmall = 1;
	maxRowOfBig = 1;
	maxColOfSmall = 1;
	maxColOfBig = 1;
	levelDone = false;
	isSmallSpaceShipExist = false;
	isBigSpaceShipExist = false;
	setBig = false;
	setSmall = false;
	ship[Small].set('@');
	ship[Small].setDirection(Direction::STAY);
	ship[Small].setColor(Color::LIGHTGREEN);

	ship[Big].setDirection(Direction::STAY);
	ship[Big].set('#');
	ship[Big].setColor(Color::YELLOW);

	if (!PlayingSavedGame)
	{
		keysEnter.clear();
		PlayLevel();
	}
		
}
void TheShipsGame::PlayLevel()
{
	if (playingLevel == numLevels)
	{
		FinishedScreens = true;
		return;
	}
	while (playingLevel < numLevels && screens[playingLevel]->isValid == false)//screen not valid and didnt print yet the errors
	{
			playingLevel++;
	}
	if (playingLevel == numLevels)
	{
		FinishedScreens = true;
		return;
	}
	else if (screens[playingLevel]->isValid)
	{
		set_board(screens[playingLevel]->getBoard());
		initBoard();
	}
}
void TheShipsGame::ScreencheckInit()
{
	ship[Small].init();
	ship[Big].init();
	for (list<Item>::iterator itr = item.begin(); itr != item.end(); ++itr)
		itr->init();
	item.clear();
	numOfItems = 0;
	totalTime = 0;
	game_over = false;
	inValidScreen = false;
	maxRowOfSmall = 1;
	maxRowOfBig = 1;
	maxColOfSmall = 1;
	maxColOfBig = 1;
	levelDone = false;
	isSmallSpaceShipExist = false;
	isBigSpaceShipExist = false;
	setBig = false;
	setSmall = false;
	ship[Small].set('@');
	ship[Small].setDirection(Direction::STAY);
	ship[Small].setColor(Color::LIGHTGREEN);

	ship[Big].setDirection(Direction::STAY);
	ship[Big].set('#');
	ship[Big].setColor(Color::YELLOW);
}
bool TheShipsGame::checkBoard(int i)
{
	bool big = true, sml = true;
	if (!ship[Big].checkSpaceShip(board, bigSize))
	{
		screens[i]->pushError("Big Ship is not valid");
		big = false;
	}
	if (!ship[Small].checkSpaceShip(board, smallSize))
	{
		screens[i]->pushError("Small ship is not valid");
		sml = false;
	}
	if (!big || !sml)
	{
		screens[i]->isValid = false;
		return false;
	}
	return true;	
}
void TheShipsGame::initScreenCheckerBoard()
{
	makeBoardOfItems();
	setTextColor(Color::WHITE);
	for (int i = 0; i < ROWS; ++i) {
		for (int j = 0; j < COLS; ++j) {
			gotoxy(j, i);
			setTextColor(Color::LIGHTGREEN);
			if (originalBoard[i][j] == 'S')
			{
				j += 24;
				xTime = j, yTime = i;
				
			}
			setTextColor(Color::WHITE);
			board[i][j] = originalBoard[i][j];
			handleObjectCreationFromBoard(i, j);
			handleItemCreationFromBoard(i, j);
		}
		board[i][COLS] = '\0';
	}
}

void TheShipsGame::printBoard()
{
	clear_screen();
	for (int i = 0; i<ROWS; ++i) {
		for (int j = 0; j < COLS; ++j) {
			setTextColor(Color::LIGHTGREEN);
			if (originalBoard[i][j] == 'X')
			{
				board[i][j++] = 'X';
				screens[playingLevel]->printMath();
				for (int k = 0; k < 5; k++, j++) {
					board[i][j] = ' ';
				}
			}
			if (originalBoard[i][j] == 'S')
			{
				board[i][j++] = 'S';
				cout << "Level: " << playingLevel + 1 << " Life: " << Life << " Time: ";
				for (int k = 0; k < 23; k++, j++) {
					board[i][j] = ' ';
				}
				xTime = j, yTime = i;
			}
			setTextColor(Color::WHITE);
			gotoxy(j, i);
			cout << board[i][j];
			cout.flush();
		}
	}
}
void TheShipsGame::handleItemCreationFromBoard(int row, int col)
{
	char ch = originalBoard[row][col];

	if (ch == '*' && boardOfItems[row][col] == 0)
	{
		Item newItem1;
		createItem(ch, row, col, newItem1);
		item.push_back(newItem1);
		numOfItems++;
	}
	else if (ch == '%'&& boardOfItems[row][col] == 0)
	{
		NumEaters newItem1;
		createItem(ch, row, col, newItem1);
		newItem1.insertPos(col, row);
		EatNums.push_back(newItem1);
		numOfEaters++;
	}
	else if ((ch == '!' || ch == '$') && boardOfItems[row][col] == 0)
	{
		Flyers fly;
		fly.setCH(ch);
		createItem(ch, row, col, fly);
		FlyList.push_back(fly);
		numOfFlyers++;
	}
}

void TheShipsGame::createItem(char ch, int row, int col, Item& item)
{
	item.insertPoint(row, col);
	boardOfItems[row][col] = 1;
	if (ch == '%' || ch == '!' || ch == '$')
		return;
	if (originalBoard[row - 1][col] == ch) {//check above
		if (boardOfItems[row - 1][col] == 0)
			createItem(ch, row - 1, col, item);
	}
	if (originalBoard[row + 1][col] == ch) {//check below
		if (boardOfItems[row + 1][col] == 0)
			createItem(ch, row + 1, col, item);
	}

	if (originalBoard[row][col + 1] == ch) {//check right
		if (boardOfItems[row][col + 1] == 0)
			createItem(ch, row, col + 1, item);
	}

	if (originalBoard[row][col - 1] == ch) {//check left
		if (boardOfItems[row][col - 1] == 0)
			createItem(ch, row, col - 1, item);
	}
	if (originalBoard[row - 1][col - 1] == ch) {//check top left
		if (boardOfItems[row - 1][col-1] == 0)
			createItem(ch, row - 1, col-1, item);
	}

	if (originalBoard[row - 1][col + 1] == ch) {//check top right
		if (boardOfItems[row - 1][col+1] == 0)
			createItem(ch, row - 1, col+1, item);
	}

	if (originalBoard[row + 1][col - 1] == ch) {//check bottom left
		if (boardOfItems[row+1][col - 1] == 0)
			createItem(ch, row+1, col - 1, item);
	}

	if (originalBoard[row + 1][col + 1] == ch) {//check bottom right
		if (boardOfItems[row+1][col + 1] == 0)
			createItem(ch, row+1, col + 1, item);
	}
}
void TheShipsGame::handleObjectCreationFromBoard(int row, int col)
{
	char ch = board[row][col];

	if (ch == '@' && !ship[Small].checkIfExist(row, col)){
		if (ship[Small].getSize() == 0){
			maxRowOfSmall = row;
			maxColOfSmall = col;
		}
		ship[Small].addParts();
		createSpaceShip(ch, row, col, Small, maxRowOfSmall, maxColOfSmall);
	}
	else if (ch == '#' && !ship[Big].checkIfExist(row, col)){
		if (ship[Big].getSize() == 0){
			maxRowOfBig = row;
			maxColOfBig = col;
		}
		ship[Big].addParts();
		createSpaceShip(ch, row, col, Big, maxRowOfBig, maxColOfBig);
	}
}
void TheShipsGame::createSpaceShip(char ch, int row, int col,int size,int& row2,int& col2)
{
	int temp = row;
		if (row - 1 == -1)
			temp = 0;
		if (originalBoard[row % 24][col % 80] == ch) {
			if (!ship[size].checkIfExist(row, col)) {
				ship[size].insertPoint(row % 24, col % 80);
				if (col > col2)
				{
					col2 = col;
					ship[size].addCols();
				}
				if (row > row2) {
					row2 = row;
					ship[size].addRows();
				}
			}
			if (originalBoard[(row + 1) % 24][col % 80] == ch) {//Check below
				if (!ship[size].checkIfExist((row + 1) % 24, col % 80))
					createSpaceShip(ch, (row + 1) % 24, col % 80, size,row2,col2);
			}
			if (originalBoard[(temp) % 24][col % 80] == ch) {//Check above
				if (!ship[size].checkIfExist((temp) % 24, col % 80))
					createSpaceShip(ch, (temp) % 24, col % 80, size, row2, col2);
			}
			if (originalBoard[row % 24][(col + 1) % 80] == ch) {//Check right
				if (!ship[size].checkIfExist(row % 24, (col + 1) % 80))
					createSpaceShip(ch, row % 24, (col + 1) % 80, size, row2, col2);
			}
			if (originalBoard[row % 24][(col - 1) % 80] == ch) {//Check left
				if (!ship[size].checkIfExist(row % 24, (col - 1) % 80))
					createSpaceShip(ch, row % 24, (col - 1) % 80, size, row2, col2);
			}
		}
}
void TheShipsGame::makeBoardOfItems()
{
	for (unsigned int i = 0; i < ROWS; i++) {
		for (unsigned int j = 0; j < COLS; j++)
		{
			boardOfItems[i][j]=0;
		}
	}
}
void TheShipsGame::setBoard(const char* boardToCopy[ROWS]) {
//The function recives a board to copy.
//the function copies the board to the game board of the class.
	// copy board sent to us into originalBoard data member
	for (int i = 0; i < ROWS; ++i) {
		for (int j = 0; j < COLS; ++j)
		{
			originalBoard[i][j] = boardToCopy[i][j];
		}
		originalBoard[i][COLS] = '\0';
	}
}
void TheShipsGame::set_board(char **boardToCopy)
{//The function recives a board to copy.
//the function copies the board to the game board of the class.
	// copy board sent to us into originalBoard data member
	for (int i = 0; i < ROWS; ++i) {
		for (int j = 0; j < COLS; ++j)
		{
			originalBoard[i][j] = boardToCopy[i][j];
		}
		originalBoard[i][COLS] = '\0';
	}
}
void TheShipsGame::setInstructionBoard(const char* boardToCopy[ROWS])
{//The function recives a board to copy
//The function copies the board to the instructionBoard of the class
	for (int i = 0; i < ROWS; ++i) {
		for (int j = 0; j < COLS; ++j)
			instructionBoard[i][j] = boardToCopy[i][j];
		instructionBoard[i][COLS] = '\0';
	}
}
string TheShipsGame::convertCharToString(char ch)
{
	stringstream ss;
	string temp;
	ss << ch;
	ss >> temp;
	return temp;
}
void TheShipsGame::printDetails(int& round)
{
	setTextColor(Color::LIGHTGREEN);
	gotoxy(xTime, yTime);
	cout << round++;
	gotoxy(xMath, yMath);
	screens[playingLevel]->printMath();
}
void TheShipsGame::doIteration(const list<char>& keyHits) {
	//The function recives a const by ref list (the movement keys)
	//the function makes each iteration of the game, by getting the relevant direction, using isWall function, and using WhatToDo function.
	int direction;
	Point next;
	printDetails(totalTime);
    if ((direction = getDirection(0, keyHits)) != -1 ) {//Small Space-Ship
		if (direction != Direction::TILT)
		{
			insertKeys(totalTime,keysEnter, keyHits,0);
			ship[Small].setDirection(direction);
			LastKeyPressedSmall = direction;
		}
		else
		{
			insertKeys(totalTime, keysEnter, keyHits,0);
			ship[Small].setDirection(direction);
			obstacle = ship[Small].isWall(board);
			WhatToDo(obstacle, ship, Small);
			ship[Small].setDirection(LastKeyPressedSmall);
		}
	}
	if ((direction = getDirection(1, keyHits))!=-1 ) {//Big space-Ship
		if (direction != Direction::TILT)
		{
			insertKeys(totalTime, keysEnter, keyHits,1);
			ship[Big].setDirection(direction);
			LastKeyPressedBig = direction;
		}
		else
		{
			insertKeys(totalTime, keysEnter, keyHits,1);
			ship[Big].setDirection(direction);
			obstacle = ship[Big].isWall(board);
			WhatToDo(obstacle, ship, Big);
			ship[Big].setDirection(LastKeyPressedBig);
		}
	}
	for (unsigned int p = 0; p < numOfItems; p++)
	{
		obstacle = get_item_i(p).isWall(board);
		WhatToDoItem(obstacle, p);
	}
	for (unsigned int p = 0; p < numOfFlyers; p++)
	{
		obstacle = get_flyer_i(p).isWall(board);
		WhatToDoFlyer(obstacle, p);
	}
	for (unsigned int p = 0; p < numOfEaters; p++)
	{
		obstacle = get_eater_i(p).isWall(board);
		WhatToDoEater(obstacle, p);
	}
	if (Life > 0) {
		obstacle = ship[Small].isWall(board);
		WhatToDo(obstacle, ship, Small);
		obstacle = ship[Big].isWall(board);
		WhatToDo(obstacle, ship, Big);
	}
	else if (Life==0)
	{
		Life = 3;
		game_over = true;
	}
}
void TheShipsGame::insertKeys(int clock_itr, vector <keyPoint> &moves,const list<char>& keyHits,int index)
{
	string key;
	keyPoint point(clock_itr);
	key.append(convertCharToString(convertDirection(keyHits,index)));
	point.insertChar(key);
	moves.push_back(point);
}
void TheShipsGame::startLevel()
{//The function recives nothing
//The function makes initialization and moving current level one step forward
	init();
}
bool TheShipsGame::IsItemOnTop(SpaceShip &ship, int size, Item &item)
{//The function recives a reference to a ship, it's size, and a reference to an item.
//the function checks if there's an item sitting on the ship, if it does the function returns true, else, returns false
	int prev_dir, itemDir;
	int itemSize = item.getSize();
	bool itemOnTop = false;
	prev_dir = ship.getDirection();
	ship.setDirection(Direction::UP);//up;
	itemDir = item.getDirection();
	if (ship.isWall(board) != 3)
	{
		ship.flagItemOnTop = false;
	}
	if (ship.isWall(board) != 3 && item.isWall(board) == false)
	{
		item.flagItemOnTop = false;
		ship.flagItemOnTop = false;
	}
	if ((ship.isWall(board) == 3 || ship.isWall(board) ==5) && item.getDirection() != 1)//item is on top and not falling.
	{
		item.setDirection(Direction::DOWN);//down
		if (item.isWall(board) == 2 || item.isWall(board) == 3)
		{
			itemOnTop = true;
			item.flagItemOnTop = true;
			ship.flagItemOnTop = true;
		}
		item.setDirection(prev_dir);
	}
	if (prev_dir == 1 && item.flagItemOnTop && ship.flagItemOnTop)//item is on top and falling (direction down)
	{
		item.setDirection(prev_dir);
		ship.flagItemOnTop = true;
		itemOnTop = true;
	}
	ship.setDirection(prev_dir);
	return itemOnTop;
}
void TheShipsGame::printInstructionsBoard(Color c)
{//The function recives nothing
//The function prints the instructionBoard- actully it's the "graphic board" of instruction, welcome screen, main menu screen and ect.
	gotoxy(0, 0);
	setTextColor(c);
	for (int i = 0; i < ROWS; ++i){
		for (int j = 0; j < COLS; ++j)
		{

			gotoxy(j, i);
			cout << instructionBoard[i][j];
			cout.flush();
		}
	}
	setTextColor(Color::WHITE);
}
void TheShipsGame::CheckSpecificScreen(int i)
{//after we got the screens.
	if (screens[i]->isValid) {
		cout << "Screen ID number: " << screens[i]->getID() << " is Valid!" << endl;
	}
	else
	{
		cout << "Screen ID number: " << screens[i]->getID() << " is Invalid! and these are the causes:" << endl;
		screens[i]->printErrors();
	}
	cout << "Press any key to go back to the main menu...";
	_getch();
}
void TheShipsGame::ScreenChecker()
{//after we got the screens.
	int IdChecker;
	bool checker = true;
	bool checker2 = true;
	for (unsigned int i = 0; i < numOfScreens; ++i)
	{
		if (!screens[i]->isChecked())//not checked yet
		{
			set_board(screens[i]->getBoard());
			ScreencheckInit();
			initScreenCheckerBoard();
			checker2=screens[i]->checkScreen();
			checker = checkBoard(i);
			if (checker && checker2)
			{
				screens[i]->convertParameters();
				screens[i]->copyMathEquition();
				screens[i]->eraseMathEquition();
			}
			else
			{
				screens[i]->isValid = false;
			}
			screens[i]->Checked();//updating boolean character, so we know we checked the screen already
		}
	}
	sort_screens(screens, numOfScreens);
	for (unsigned int i = 0; i < numOfScreens - 1; ++i)
	{
		if (screens[i]->getID() == screens[i + 1]->getID())
		{
			IdChecker = screens[i]->getID();
			screens[i]->pushError("The screen ID is token by other screen");
			screens[i]->isValid = false;
			i++;
			while (screens[i]->getID() == IdChecker)
			{
				screens[i]->pushError("The screen ID is token by other screen");
				screens[i]->isValid = false;
				i++;
			}
		}
	}
}
void TheShipsGame::getScreens()
{
	string FileName;
	string screen_id;
	string math;
	numOfScreens = getNumOfScreens();
	screens = new Screens*[numOfScreens];
	char **board;
	ifstream FileNames("spgFilesName.txt");
	for (unsigned int i = 0; i < numOfScreens; ++i)
	{
		getline(FileNames, FileName);
		fstream input(FileName);
		getline(input, screen_id);//getting screen id
		getline(input, math);//getting math expression
		getBoardFromFile(input,board);//getting board
		screens[i] = new Screens(FileName,screen_id, math, board);//creating a screen
		input.close();
	}
}

void TheShipsGame::getBoardFromFile(fstream& input, char ** &board)const
{
	string trash;
	int i, j;
	char ch;
	bool stop = false;
	board = new char*[ROWS];
	for (i = 0; i<ROWS; i++)
		board[i] = new char[COLS+1];

	for (i = 0; i<ROWS; i++){
		for (j = 0; j<COLS && !stop; j++){
			input.get(ch);
			if (ch == '\n'){
				stop = true;
				j--;
			}
			else
				board[i][j] = ch;
		}
		if (j<COLS)
			for (; j<COLS; j++)
				board[i][j] = ' ';
		else{
			getline(input, trash);
		}
		board[i][COLS] = '\0';
		stop = false;
	}
	clear_screen();
}
void TheShipsGame::saveGame(const string& name)
{
	string id="";
	totalTime++;
	int size;
	keyPoint Point(totalTime);
	string key = "sk";
	Point.insertChar(key);
	keysEnter.push_back(Point);

	//int size = screens[playingLevel]->getScreenId().size();
	if (!name.empty())
	{
		id.insert(0, screens[playingLevel]->getScreenId());
		id.erase(0, 9);
		size = id.size();
		id.insert(size, ".spd");
	}
	else
	{
		clear_screen();
		cout << "Please enter the name of the file: " << endl;
		getline(cin,id);
		size = id.size();
		id.insert(size, ".spp");
	}
	ofstream newFile(id);
	int i, j;
	newFile <<screens[playingLevel]->getScreenId() << endl;
	newFile << "ClockIterations=" << totalTime << endl;
	if (!name.empty())
		newFile << "NameOfSolver="<<name << endl;
	newFile << screens[playingLevel]->getMath() << endl;
	newFile << "Life:" << Life << endl;
	for (i = 0; i < ROWS; i++) {
		for (j = 0; j < COLS; j++)
		{
			newFile << board[i][j];
		}
		newFile << endl;
	}
	newFile << keysEnter.size() << endl;
	for (i = 0; i < keysEnter.size(); i++)
		newFile << keysEnter[i].getClock() << ":" << keysEnter[i].getKey()<< endl;

	newFile.close();
	system("dir /b *.spp > sppFiles.txt");
}
int TheShipsGame::numOfSavedGames()
{
	string res;
	int counter = 0;
	system("dir /b *.spp > sppFiles.txt");
	ifstream f("sppFiles.txt");
	if (f.is_open())
	{
		clear_screen();
		while (!f.eof())
		{
			getline(f, res);
			counter++;
		}
	}
	if (counter != 0)
		counter--;
	f.close();
	return counter;
}
void TheShipsGame::initBoard()
{//The function recives nothing
	//The function initialization the board

	makeBoardOfItems();
	setTextColor(Color::WHITE);
	bool printedS = false, printedX = false;
	// copy original board to actual board and build all the relevant objects
	// this shall be called any time we Start the level
	for (int i = 0; i < ROWS; ++i) {
		for (int j = 0; j < COLS; ++j) {
			gotoxy(j, i);
			setTextColor(Color::LIGHTGREEN);
			if (originalBoard[i][j] == 'X' && !printedX)
			{
				printedX = true;
				xMath = j; yMath = i;
				board[i][j++] = 'X';
				screens[playingLevel]->printMath();
				for (int k = 0; k < 5; k++, j++) {
					board[i][j] = ' ';
				}
			}

			if (originalBoard[i][j] == 'S' && !printedS)
			{
				printedS = true;
				board[i][j++] = 'S';
				cout << "Level: " << playingLevel + 1 << " Life: " << Life << " Time: ";
				for (int k = 0; k < 23; k++, j++) {
					board[i][j] = ' ';
				}
				xTime = j, yTime = i;
			}
			setTextColor(Color::WHITE);
			cout << originalBoard[i][j];
			cout.flush();
			board[i][j] = originalBoard[i][j];
			handleObjectCreationFromBoard(i, j);
			handleItemCreationFromBoard(i, j);
		}
		board[i][COLS] = '\0';
	}
	getNumbersPos();
	for (list<Flyers>::iterator itr = FlyList.begin(); itr != FlyList.end(); itr++)
		itr->set();
	for (list<NumEaters>::iterator itr = EatNums.begin(); itr != EatNums.end(); ++itr)
	{
		itr->set();
		FindMinDistance(*itr);
	}
}

void TheShipsGame::FindMinDistance(NumEaters &eater)
{
	int minDistance = 150;
	int distance = 0;
	int xTar, yTar;
	int xPos, yPos;
	int numPosX, numPosY;
	xPos = eater.getxPos();
	yPos = eater.getyPos();
	if (NumberPositions.empty())
	{
		eater.insertTargets(-1, -1);
		return;
	}
	for (vector<pointNum>::iterator itrNums = NumberPositions.begin(); itrNums != NumberPositions.end(); itrNums++)
	{
		numPosX = itrNums->getX();
		numPosY = itrNums->getY();
		xTar = numPosX;
		yTar = numPosY;
		while (numPosX < xPos)
		{
			numPosX++;
			distance++;
		}
		while (numPosX > xPos)
		{
			numPosX--;
			distance++;
		}
		while (numPosY < yPos)
		{
			numPosY++;
			distance++;
		}
		while (numPosY > yPos)
		{
			numPosY--;
			distance++;
		}
		if (distance < minDistance)
		{
			eater.insertTargets(xTar, yTar);
			minDistance = distance;
		}
		distance = 0;
	}
}

void TheShipsGame::getNumbersPos()
{
	int posY = 0;
	int num = 0;
	bool foundNumber = false;
	for (int i = 0; i < ROWS; ++i)
	{
		for (int j = 0; j < COLS; ++j)
		{
			posY = j;
			while ((board[i][j] >= '0' && board[i][j] <= '9') && j<COLS)
			{
				num *= 10;
				num += board[i][j] - '0';
				j++;
				foundNumber = true;
			}
			if (foundNumber)
			{
				pointNum pos(posY, i);
				NumberPositions.push_back(pos);
				foundNumber = false;
				num = 0;
				posY = 0;
				numOfNumbers++;
			}
		}
	}
}

void TheShipsGame::setSavedGame(int num)
{
	ifstream f("sppFiles.txt");
	string FileName, stringID, Exercise, stringClock;
	string stringLife;
	vector <keyPoint> moves;
	char l;
	bool found = false;
	char **board;
	int clockIter;
	int saved_life;
	int index = 1;
	int savescreenID;

	for (size_t i = 1; i <= num; i++)
		getline(f, FileName);
	fstream input(FileName);
	getline(input, stringID);
	getline(input, stringClock);
	getline(input, Exercise);//getting math expression
	getline(input, stringLife);
	clockIter = stringToInt(stringClock);
	getBoardFromFile(input,board);//getting board
	l = stringLife[5];
	saved_life = l - '0';
	saveScreen = new Screens(FileName, stringID, Exercise, board);
	getTimeVectors(input, moves);

	saveScreen->checkScreenID();
	saveScreen->Checked();
	saveScreen->convertParameters();
	saveScreen->isValid = true;
	keysEnter = moves;
	Life = saved_life;
	totalTime = clockIter;
	savescreenID = saveScreen->getID();
	for (size_t i = 0; i < numOfScreens && !found; ++i)
	{//getting playing level for next level.
		if (screens[i]->getID() == savescreenID)
		{
			playingLevel = i;
			found = true;
		}
	}
	*screens[playingLevel] = *saveScreen;
	set_board(saveScreen->getBoard());
	init();
	initBoard();
}
bool TheShipsGame::printSavedGames()
{
	clear_screen();
	int i = 1;
	string line;
	system("dir /b *.spp > sppFiles.txt");
	ifstream files("sppFiles.txt");
	if (!Is_empty(files)) {
		clear_screen();
		cout << "Select which screen you would like to play:" << endl;
		cout << endl;
		while (!files.eof())
		{
			getline(files, line);
			if(line.compare(""))
				cout << i++<<". "<<line << endl;
		}
		return true;
	}
	else
	{
		clear_screen();
		cout << "There is no saved games!" << endl;
		Sleep(800);
		return false;
	}
}
bool Is_empty(ifstream& File)
{
	return File.peek() == ifstream::traits_type::eof();
}
void TheShipsGame::printScreenNames()
{
	for (unsigned int i = 0; i < numLevels; ++i)
	{
		cout << i + 1 << "." << screens[i]->getFileName() << endl;
	}
}

int TheShipsGame::getNumOfScreens()
{
	string res;
	int counter = 0;
	system("dir /b *.spg > spgFilesName.txt");
	ifstream f("spgFilesName.txt");
	if (f.is_open())
	{
		while (!f.eof())
		{
			getline(f, res);
			counter++;
		}
	}
	if (counter!=0)
		counter--;
	f.close();
	numLevels = counter;
	return counter;
}
void TheShipsGame::sort_screens(Screens **scr, int size)
{
	Screens **bad_screensID;
	Screens **good_screensID;
	int high, low;
	unsigned int j = 0;
	int counterBadID = 0, counterGoodID = 0;
	for (unsigned int i = 0; i < size; i++)
	{
		if (scr[i]->checkScreenID() == false)
			counterBadID++;
	}
	bad_screensID = new Screens*[counterBadID];
	for (unsigned int i = 0; i < size; i++)
	{
		if (scr[i]->checkScreenID() == false)
		{
			bad_screensID[j] = scr[i];
			j++;
		}
		
	}
	counterGoodID = size - counterBadID;
	good_screensID = new Screens*[counterGoodID];
	j = 0;
	for (unsigned int i = 0; i < size; i++)
	{
		if (scr[i]->checkScreenID() == true)
		{
			good_screensID[j] = scr[i];
			j++;
		}
		
	}
	high = counterGoodID - 1;
	low = 0;
	mergesort(good_screensID, counterGoodID, low, high);
	j = 0;
	for (unsigned i = 0; i < counterGoodID; i++)
	{
		scr[j] = good_screensID[i];
		j++;
	}
	for (unsigned i = 0; i < counterBadID; i++)
	{
		scr[j] = bad_screensID[i];
		j++;
	}
	delete []good_screensID;
	delete []bad_screensID;
}
void TheShipsGame::mergesort(Screens **scr, int size, int low, int high)
{
	int mid;
	if (low < high)
	{
		mid = (low + high) / 2;
		mergesort(scr, size, low, mid);
		mergesort(scr, size, mid + 1, high);
		merge(scr, size, low, mid, high);
	}
}
void TheShipsGame::merge(Screens **scr, int size, int low, int mid, int high)
{
	int i, j, k;
	Screens **temp = new Screens*[size];
	for (i = 0; i < size; i++)
		temp[i] = scr[i];
	i = low;
	j = mid + 1;
	k = low;
	while (i <= mid && j <= high)
	{
		if (temp[i]->getID() < temp[j]->getID())
		{
			scr[k] = temp[i];
			i++;
			k++;
		}
		else
		{
			scr[k] = temp[j];
			j++;
			k++;
		}
	}
	while (i <= mid)
	{
		scr[k] = temp[i];
		k++;
		i++;
	}
	while (j <= high)
	{
		scr[k] = temp[j];
		k++;
		j++;
	}
	delete temp;

}
void TheShipsGame::welcome(){//welcome screen
	clear_screen();
	const char* WelcomeScreen[ROWS] = {
	//	         10        20        30        40        50        60        70       79
	//	01234567890123456789012345678901234567890123456789012345678901234567890123456789
		"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++", // 0
		"+                              WELCOME TO THE:                                 +", // 1
		"+ _____  _               ____                           _____                  +", // 2
		"+|_   _|| |             / ___|                         /  _  \\                 +", // 3
		"+  | |  | |__   ____   | |___  ____  __ _  ___  ____   | | |_/ __ _ _ _ _  ___ +", // 4
		"+  | |  |  _ \\ / __ \\  \\____ \\| __ \\/ _' |/ __|/ __ \\  | | __ / _' | ' ' |/ _ \\+", // 5
		"+  | |  | | | |   __/   ___/  |  __/|(_| | |__ |  __/  | |_\\ \\|(_) | | | |  __/+", // 6
		"+  |_|  |_| |_|\\____|	 |_____/|_|   \\__,_|\\___|\\____|  \\_____/\\__,_|_|_|_|\\___|+", // 7
		"+                                                                              +", // 8
		"+                       BY IDAN DAGAN & DANIEL FRUEND                          +", // 9
		"+                                                                              +", // 10
		"+                       Menu:                                                  +", // 11
		"+                       Press 1 --> Instructions.                              +", // 12
		"+                       Press 2 --> Play The Game.                             +",
		"+                       Press 3 --> Use the screen checker.                    +",
		"+                       Press 4 --> Continue playing from saved screen.        +",
		"+                       Press 5 --> Play from selected screen.                 +", // 14
		"+                       Press 9 --> Exit.                                      +", // 15
		"+                                                                              +", // 16
		"+                                                                              +", // 17
		"+                                                                              +", // 18
		"+                                                                              +", // 19
		"+                                                                              +", // 20
		"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
   //   01234567890123456789012345678901234567890123456789012345678901234567890123456789
	};
	setInstructionBoard(WelcomeScreen);
	printInstructionsBoard(LIGHTCYAN);
	/*MakeBeep(300, 400);
	MakeBeep(400, 400);
	MakeBeep(500, 400);
	MakeBeep(600, 600);
	MakeBeep(500, 300);
	MakeBeep(600, 400);*/
}
void TheShipsGame::gameover() {//welcome screen
	clear_screen();
	const char* Gameover[ROWS] = {
		//	         10        20        30        40        50        60        70       79
		//	01234567890123456789012345678901234567890123456789012345678901234567890123456789
		"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++", // 0
		"+                                                                              +", // 1
		"+                      _____                                                   +", // 2
		"+                     /  _  \\                                                  +", // 3
		"+                     | | |_/ __ _ _ _ _  ____                                 +", // 4
		"+                     | | __ / _' | ' ' |/ __ \\                                +", // 5
		"+                     | |_\\ \\|(_) | | | |  ___/                                +", // 6
		"+                     \\_____/\\__,_|_|_|_|\\____|                                +", // 7
		"+                                                                              +", // 8
		"+                                _____                                         +", // 9
		"+                               /     \\                                        +", // 10
		"+                               |  _  |__    __ ____   _ ____                  +", // 11
		"+                               | |_| |\\ \\  / // __ \\ | '____\\                 +", // 12
		"+                               |     | \\ \\/ / | ___/ | /                      +", // 13
		"+                               \\_____/  \\__/  \\____| |_|                      +", // 14
		"+                                                                              +", // 15
		"+                                                                              +", // 16
		"+                                                                              +", // 17
		"+                                                                              +", // 18
		"+                                                                              +", // 19
		"+                                                                              +", // 20
		"+                                                                              +", // 21
		"+                                                                              +", // 22
		"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"  // 23
																							//   01234567890123456789012345678901234567890123456789012345678901234567890123456789
	};

	setInstructionBoard(Gameover);
	printInstructionsBoard(RED);
	/*MakeBeep(500, 500);
	MakeBeep(400, 500);
	MakeBeep(300, 500);
	MakeBeep(100, 500);*/
	Sleep(2000);
}
void TheShipsGame::welldone() {//welcome screen
	clear_screen();
	const char* Welldone[ROWS] = {
		//	         10        20        30        40        50        60        70       79
		//	01234567890123456789012345678901234567890123456789012345678901234567890123456789
		"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++", // 0
		"+                                                                              +", // 1
		"+                                                                              +", // 2
		"+                                    _  _                                      +", // 3
		"+                 __    _    __ ___ | || |                                     +", // 4
		"+                 \\ \\  / \\  / // _ \\| || |                                     +", // 5
		"+                  \\ \\/   \\/ / | __/| || |                                     +", // 6
		"+                   \\__/ \\__/  \\___||_||_|                                     +", // 7
		"+                             ____                                             +", // 8
		"+                            |    \\ ___  ___   ___     _                       +", // 9
		"+                            | || |/   \\/   \\ / _ \\   | |                      +", // 10
		"+                            | || |  | || || |  __/   | |                      +", // 11
		"+                            |____/\\___/|_||_|\\___|   |_|                      +", // 12
		"+                                                      _                       +", // 13
		"+                                                     |_|                      +", // 14
		"+                                                                              +", // 15
		"+                                                                              +", // 16
		"+                                                                              +", // 17
		"+                                                                              +", // 18
		"+                                                                              +", // 19
		"+                                                                              +", // 20
		"+                                                                              +", // 21
		"+                                                                              +", // 22
		"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"  // 23
																							//   01234567890123456789012345678901234567890123456789012345678901234567890123456789
	};
	setInstructionBoard(Welldone);
	printInstructionsBoard(LIGHTGREEN);
	/*MakeBeep(300, 400);
	MakeBeep(400, 400);
	MakeBeep(500, 400);
	MakeBeep(600, 600);
	MakeBeep(500, 300);
	MakeBeep(600, 400);*/
}
void TheShipsGame::instructions() {//instructions
	clear_screen();
	const char* instructions[ROWS] = {
	//	         10        20        30        40        50        60        70       79
	//	01234567890123456789012345678901234567890123456789012345678901234567890123456789
		"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++", // 0
		"+                               INSTRUCTIONS:                                  +", // 1
		"+                                                                              +", // 2
		"+    Game Target: You must pick the right number that solves the equation.     +", // 3
		"+   **If there is a gate, so you need to get a key to open it. Marked by 'K'** +", // 4
		"+              Left Player:              Right Player:                         +", // 13
		"+                 ____                       ____                              +", // 14
		"+                |    |                     |    |                             +", // 15
		"+            ____|__W_|____             ____|__I_|____                         +", // 16
		"+           |    |    |    |           |    |    |    |                        +", // 17
		"+           |__A_|__S_|__D_|           |__J_|__K_|__L_|                        +", // 18
		"+           |    |    |                |    |    |                             +", // 19
		"+           |__Z_|__X_|                |__N_|__M_|                             +", // 12
		"+                                                                              +", // 13
		"+           W = Up.                     I = Up.                                +", // 14
		"+           S = Down.                   M = Down.                              +", // 15
		"+           A = Left.                   J = Left.                              +", // 16
		"+           D = Right.                  L = Right.                             +", // 17
		"+           X = Down.                   M = Down.                              +", // 18
		"+           Z = Rotate.                 N = Rotate.                            +", // 19
		"+                                                                              +", // 20
		"+	                 Press Any Key -> Go Back.                                   +", // 21
		"+                                                                              +", // 22
		"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"  // 23
	//   01234567890123456789012345678901234567890123456789012345678901234567890123456789
	};

	setInstructionBoard(instructions);
	printInstructionsBoard(LIGHTCYAN);
}
void TheShipsGame::BackToMenu() {//backToMenu

	const char* MainMenu[ROWS] = {
		//	         10        20        30        40        50        60        70       79
		//	01234567890123456789012345678901234567890123456789012345678901234567890123456789
		"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++", // 0
		"+                              WELCOME TO THE:                                 +", // 1
		"+ _____  _               ____                           _____                  +", // 2
		"+|_   _|| |             / ___|                         /  _  \\                 +", // 3
		"+  | |  | |__   ____   | |___  ____  __ _  ___  ____   | | |_/ __ _ _ _ _  ___ +", // 4
		"+  | |  |  _ \\ / __ \\  \\____ \\| __ \\/ _' |/ __|/ __ \\  | | __ / _' | ' ' |/ _ \\+", // 5
		"+  | |  | | | |   __/   ___/  |  __/|(_| | |__ |  __/  | |_\\ \\|(_) | | | |  __/+", // 6
		"+  |_|  |_| |_|\____|	  |_____/|_|   \\__,_|\\___|\\____|  \\_____/\\__,_|_|_|_|\\___|+", // 7
		"+                                                                              +", // 8
		"+                       BY IDAN DAGAN & DANIEL FRUEND                          +", // 9
		"+                                                                              +", // 10
		"+                       Menu:                                                  +", // 11
		"+                       Press 1 --> Exit Application.                          +", // 12
		"+                       Press 2 --> Back To Main Menu.                         +", // 13
		"+                       Press 3 --> Continue.                                  +", // 14
		"+                       Press 4 --> Replay Level.                              +", // 15
		"+                       Press 5 --> Save Game.                                 +", // 16
		"+                       Press 6 --> Show Solution.                             +", // 17
		"+                                                                              +", // 18
		"+                                                                              +", // 19
		"+                                                                              +", // 20
		"+                                                                              +", // 21
		"+                                                                              +", // 22
		"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"  // 23
		//   01234567890123456789012345678901234567890123456789012345678901234567890123456789
	};

	setInstructionBoard(MainMenu);
	printInstructionsBoard(LIGHTCYAN);
}
void TheShipsGame::FinishGame() {//backToMenu

	const char* finishGame[ROWS] = {
		//	         10        20        30        40        50        60        70       79
		//	01234567890123456789012345678901234567890123456789012345678901234567890123456789
		"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++", // 0
		"+                                                                              +", // 1
		"+                      _____                                                   +", // 2
		"+                     /  _  \\                                                  +", // 3
		"+                     | | |_/ __ _ _ _ _  ____                                 +", // 4
		"+                     | | __ / _' | ' ' |/ __ \\                                +", // 5
		"+                     | |_\\ \\|(_) | | | |  ___/                                +", // 6
		"+                     \\_____/\\__,_|_|_|_|\\____|                                +", // 7
		"+                                                                              +", // 8
		"+                      ____                                                    +", // 9
		"+                     |    \\   ___   ____  ____                                +", // 10
		"+                     |  |  | /   \\ /    \\/ __ \\                               +", // 11
		"+                     |  |  | | | | | || || ___/                               +", // 12
		"+                     |____/  \\___/ |_||_|\\____|                               +", // 13
		"+                                                                              +", // 14
		"+                                                                              +", // 15
		"+                                                                              +", // 16
		"+                  Thanks For playing The Ships Game!                          +", // 17
		"+                                                                              +", // 18
		"+                              Bye Bye!                                        +", // 19
		"+                                                                              +", // 20
		"+                                                                              +", // 21
		"+                                                                              +", // 22
		"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"  // 23																		//   01234567890123456789012345678901234567890123456789012345678901234567890123456789
	};
	for (int i = 0; i < 5; i++)
	{
		setInstructionBoard(finishGame);
		printInstructionsBoard(LIGHTGREEN);
		Sleep(500);
		printInstructionsBoard(LIGHTCYAN);
		Sleep(500);
		printInstructionsBoard(LIGHTMAGENTA);
		Sleep(500);
		printInstructionsBoard(LIGHTGREY);
		Sleep(500);
		printInstructionsBoard(LIGHTBLUE);
		Sleep(500);
	}
}