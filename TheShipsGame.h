#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_


#include "ISpecificGame.h"
#include "SpaceShip.h"
#include "item.h"
#include "io_utils.h"
#include <list>
#include <vector>
#include "Screens.h"
#include <fstream>
#include <iostream>
#include "Direction.h"
#include "numEaters.h"
#include "Flyers.h"
#include "PointNum.h"

using namespace std;
const static char ESC = 27; // the ESC key
class SpaceShip;
class Screens;


class TheShipsGame : public ISpecificGame{

	class keyPoint {
		int clock_itr;
		string key;
	public:
		keyPoint(int _clock) : clock_itr(_clock) {};
		void insertChar(string ch) { key.append(ch); };
		int getClock() { return clock_itr; };
		string getKey() { return key; };
	};
	vector<pointNum> NumberPositions;
	int numOfNumbers = 0;
	list<Flyers> FlyList;
	list<NumEaters> EatNums;
	int numOfEaters = 0;
	int numOfFlyers = 0;
	vector<keyPoint> keysEnter;
	Screens *saveScreen=nullptr;
	int NumIterations = 0;
	bool nowRunningSol = false;
	bool PlayingSavedGame = false;
	bool levelDone = false;
	bool GetKey = false, setBig=true, setSmall = true;
	int currentLevel = 1, numOfItems=0;
	int bestTime = 0;
	int numLevels = 1, playingLevel = 0;
	enum{bigSize=6, smallSize=3};
	enum{ROWS=24, COLS=80};
	int numOfScreens = 0;
	int totalTime = 0;
	SpaceShip ship[2];
	Screens **screens;
	list<Item> item;
	int obstacle,dummy;
	bool game_over = false, inValidScreen = false, FinishedScreens = false;
	bool isSmallSpaceShipExist = false, isBigSpaceShipExist = false;
	int xTime, yTime, tempTime;
	int xAnswer, yAnswer;
	int Life = 3;
	int keyPressed, LastKeyPressedBig, LastKeyPressedSmall,lastItemDirection;
	int maxRowOfSmall = 1, maxRowOfBig = 1, maxColOfSmall = 1, maxColOfBig = 1;
	int smallX, smallY, bigX, bigY;
	char boardOfItems[ROWS][COLS];
	char instructionBoard[ROWS][COLS + 1];
	char originalBoard[ROWS][COLS + 1];	// this is the original board that we got (we need COLS+1 because we handle it as null terminated char*)
	char board[ROWS][COLS + 1];	// this is the actual board we play on, i.e. changes on board are done herechar originalBoard[ROWS][COLS + 1];	// this is the original board that we got (we need COLS+1 because we handle it as null terminated char*)

public:

	//~TheShipsGame();
	TheShipsGame(){};//empty ctor
	int getNewDirection(int shipIndex, string& str);
	TheShipsGame(const TheShipsGame &g) = delete;//del copy ctor
	TheShipsGame &operator=(const TheShipsGame &p) = delete;//delete operator=
	friend bool Is_empty(ifstream& pFile);
	friend int strToInt(const string& str);
	void getTimeVectors(fstream &file, vector <keyPoint> &moves);
	string convertCharToString(char ch);
	void printDetails(int& round);
	string convertToString(int num);
	void init();
	void insertKeys(int clock_itr, vector <keyPoint> &moves, const list<char>& keyHits,int index);
	int xMath, yMath;
	void initSavedGame();
	virtual void setTime() { tempTime = totalTime; };
	void printInstructionsBoard(Color c);
	void initBoard();
	void show_solution(vector<keyPoint>& moves);
	void makeBoardOfItems();
	int checkBestTime(const string& name);
	void saveBestGame();
	friend int stringToInt(const string& string);
	void setBoard(const char* boardToCopy[ROWS]);
	void set_board(char **boardToCopy);
	void setInstructionBoard(const char* boardToCopy[ROWS]);
	void printCH(int i, int j, char ch) { board[i][j] = ch; };
	void WhatToDo(int obstacle, SpaceShip ship[],int size);
	void MoveWithItem(SpaceShip &ship, int size, Item &item, bool itemOnTop);
	void MoveWithItemToHole(SpaceShip &ship, int size, Item &item);
	void SpacePushItem(SpaceShip *ship, int size, Item &item);
	void TookWrongAnswer(SpaceShip *ship, int size);
	bool checkBoard(int i);
	void WhatToDoItem(int obstacle,int size);
	void WhatToDoEater(int obstacle, int size);
	void WhatToDoFlyer(int obstacle, int size);
	bool IsItemOnTop(SpaceShip &ship, int size, Item  &item);
	int getDirection(int shipIndex, const list<char> & keyHits);
	void handleObjectCreationFromBoard(int row, int col);
	void handleItemCreationFromBoard(int row,int col);
	void createSpaceShip(char ch, int row, int col, int size,int& row2, int& col2);
	void createItem(char ch, int row, int col, Item& item);
	int WhichItem(SpaceShip &ship, Item *item = nullptr, int itm = -1);
	Item & get_item_i(int i);
	Flyers& get_flyer_i(int i);
	NumEaters& get_eater_i(int i);
	int getNumOfScreens();
	void getBoardFromFile(fstream& input, char ** &board)const;
	void initScreenCheckerBoard();
	void sort_screens(Screens **scr, int size);
	void mergesort(Screens **scr, int size, int low, int high);
	void merge(Screens **scr, int size, int low, int mid, int high);
	void PlayLevel();
	void ScreencheckInit();
	char convertDirection(const list<char>& keyHits, int index);
	void DeleteDeadItems();
	void DeleteDeadEatnums();
	void getNumbersPos();
	void FindMinDistance(NumEaters &eater);
	void DeleteNum(int x,int y);

	virtual void initLife(){ Life = 3; }
	virtual void FreeSaveScreen(); 
	virtual bool IfPlayingSavedGame(){ return PlayingSavedGame; }
	virtual void CPlayingSavedGame(bool b){ PlayingSavedGame = b; }
	virtual void ChangeLevelDone(bool b){ levelDone = b; }
	virtual void CFinishedScreens(bool b){ FinishedScreens = b; }
	virtual void RunSol(bool b){ nowRunningSol = b; }
	virtual int getPlayingLevel(){ return playingLevel; }
	virtual void getBestMove();
	virtual int numOfSavedGames();
	virtual void erase() { Life = 3, totalTime = 0; };
	virtual bool isFinishedScreens(){ return FinishedScreens; }
	virtual void CheckSpecificScreen(int i);
	virtual void initCurrentLevel(){ playingLevel = 0; }
	virtual bool isLevelDone(); 
	virtual bool hasNextLevel()const{ return playingLevel < numLevels; }
	virtual void startLevel();
	virtual void doIteration(const list<char>& keyHits);
	virtual void doSubIteration(){}
	virtual void printBoard();
	virtual void welcome();
	virtual void FinishGame();
	virtual void welldone();
	virtual void gameover();
	virtual void BackToMenu();
	virtual void instructions();
	virtual void getScreens();
	virtual void ScreenChecker();
	virtual void saveGame(const string& name= "");
	virtual bool isGameOver(){ return game_over; }
	virtual bool isinValidScreen(){ return inValidScreen; }
	virtual bool isScreenIvalid(int i);
	virtual void FreeScreens();
	virtual void LevelIsDone();
	virtual void setSpecificLevel(int num){ playingLevel = num-1; }
	virtual void setSavedGame(int num);
	virtual void printScreenNames();
	virtual int getNumLevels(){ return numLevels; }
	virtual bool printSavedGames();
};

#endif
