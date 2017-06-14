#ifndef _ISPECIFIC_GAME_H_
#define _ISPECIFIC_GAME_H_

#include <list>
#include <vector>
using namespace std;
// this is a base class for actual games
// actual games will inherit from this class and implement the required methods
class ISpecificGame
{
public:
	// virtual fucnction with =0 is called an "abstract method"
	// abstract method must be implemented by non-abstract inherited class
	virtual bool isLevelDone()=0;
	virtual bool hasNextLevel()const=0;
	virtual void startLevel()=0;
	virtual void welcome()=0;
	virtual void welldone() = 0;
	virtual void FinishGame()=0;
	virtual void gameover()=0;
	virtual void BackToMenu()=0;
	virtual void printBoard() = 0;
	virtual void instructions()=0;
	virtual void initCurrentLevel() = 0;
	virtual bool isGameOver() = 0;
	virtual void saveGame(const string& name = "") = 0;
	virtual int numOfSavedGames() = 0;
	virtual bool printSavedGames() = 0;
	virtual void FreeScreens() = 0;
	virtual void LevelIsDone() = 0;
	virtual void setSavedGame(int num) = 0;
	virtual void ScreenChecker() = 0;
	virtual void erase() = 0;
	virtual void setTime() = 0;
	virtual void getScreens() = 0;
	virtual void setSpecificLevel(int num) = 0;
	virtual void printScreenNames() = 0;
	virtual int getNumLevels() = 0;
	virtual bool isinValidScreen() = 0;
	virtual void getBestMove() = 0;
	virtual void CheckSpecificScreen(int i) = 0;
	virtual bool isFinishedScreens() = 0;
	virtual int getPlayingLevel() = 0;
	virtual void RunSol(bool b) = 0;
	virtual void ChangeLevelDone(bool b) = 0;
	virtual bool IfPlayingSavedGame() = 0;
	virtual void CPlayingSavedGame(bool b) = 0;
	virtual void FreeSaveScreen() = 0;
	virtual bool isScreenIvalid(int i) = 0;
	virtual void initLife() = 0;
	virtual void CFinishedScreens(bool b) = 0;
	// get a list with keyHits and returns a list with the keys that were used
	virtual void doIteration(const list<char>& keyHits)=0;
	virtual void doSubIteration()=0;
};

#endif