#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include "ISpecificGame.h"
#include <string>
#include <list>

using namespace std;

class GameManager
{
	ISpecificGame& actualGame;
	unsigned int currentLevel;
	unsigned int clockCycleInMillisec;
	unsigned int iterationsPerClockCycle;
	const static unsigned int KEYBOARD_HIT_LOOP = 10;
	const static char ESC = 27; // the ESC key
	bool ScreenChekcerDone = false;
public:
	GameManager(ISpecificGame& theSpecificGame, unsigned int clockCycleLengthInMillisec, unsigned int numIterationsPerClockCycle)
		:actualGame(theSpecificGame), currentLevel(0), clockCycleInMillisec(clockCycleLengthInMillisec), iterationsPerClockCycle(numIterationsPerClockCycle) {}
	void run();
private:
	// helper methods
	void init()
	{
		currentLevel = 0;
	}
	char mainMenu()const;
	bool playGame();
	char playNextLevel();
	char doLevelIterations();
	bool doIteration();
	bool doInputIteration();
public:
	class MainMenuOptions
	{
	public:
		static bool isValidOption(char option) {
			string validOptions = "123459";
			return (validOptions.find(option) != string::npos);
		}
		enum
		{
			PRESENT_INSTRUCTIONS		=	'1',
			PLAY_GAME			=	'2',
			SCREEN_CHECKER = '3',
			PLAY_FROM_SAVED_SCREEN = '4',
			PLAY_FROM_SELECTED_SCREEN	=	'5',
			EXIT_APPLICATION		=	'9'
		};
	};
	class LevelOptions
	{
	public:
		static bool isValidOption(char option) {
			string validOptions = "1234569";
			return (validOptions.find(option) != string::npos);
		}
		enum
		{
			EXIT_APPLICATION			=	'1',
			BACK_TO_MAIN_MENU			=	'2',
			CONTINUE				=	'3',
			REPLAY_LEVEL				=	'4',
			SAVE_GAME					=	'5',
			SHOW_SOLUTION				=	'6',
			NEXT_LEVEL					 = '9'
		};
	};
};

#endif