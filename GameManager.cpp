#include <list>
#include "GameManager.h"
#include "ISpecificGame.h"
#include "io_utils.h"
#include <iostream>
#include <fstream>
#include <amp_graphics.h>
#define ROWS 24
#define COLS 80
using namespace std;

char GameManager::mainMenu()const
{
	actualGame.welcome();
	char selection = 0;
	do {
		selection = _getch();
	} while(!GameManager::MainMenuOptions::isValidOption(selection));
	return selection;
}
void GameManager::run()
{
	int numScreens;
	int num,temp;
	bool userWantsToPlay = true;
	// we run as long as the user wants
	while (userWantsToPlay) {
		char menuSelection = mainMenu();
		// TODO: handle here all the different menu options
		switch (menuSelection)
		{
		case GameManager::MainMenuOptions::PRESENT_INSTRUCTIONS:
			actualGame.instructions();
			_getch();//The user is pressing any key to go back to the main menu.
			break;
		case GameManager::MainMenuOptions::PLAY_GAME:
			if (ScreenChekcerDone)
			{
				actualGame.FreeScreens();
				ScreenChekcerDone = false;
			}
			actualGame.getScreens();
			clear_screen();
			numScreens = actualGame.getNumLevels();
			if (numScreens != 0)
			{


				actualGame.ScreenChecker();
				ScreenChekcerDone = true;
				actualGame.initCurrentLevel();
				init();
				userWantsToPlay = playGame();
			}
			else
			{
				cout << "There are no saved screens in the folder!" << endl;
				cout<<"Please save screens in order to run a saved game..." << endl;
				Sleep(3000);
				clear_screen();
			}
			break;
		case GameManager::MainMenuOptions::SCREEN_CHECKER:
			if (ScreenChekcerDone)
			{
				actualGame.FreeScreens();
				ScreenChekcerDone = false;
			}
			actualGame.getScreens();
			clear_screen();
			numScreens = actualGame.getNumLevels();
			if (numScreens != 0)
			{
				actualGame.ScreenChecker();
				ScreenChekcerDone = true;
				clear_screen();
				cout << "Select witch screen you want to check:" << endl;
				actualGame.printScreenNames();
				cout << "Press 0 for going back to menu...." << endl;
				cin >> num;
				cin.ignore();
				if (num != 0)
				{
					clear_screen();
					if (num > numScreens || num < 0)
					{
						cout << "Wrong input!! going back to main menu..." << endl;
						Sleep(3000);
						clear_screen();
						actualGame.FreeScreens();
						ScreenChekcerDone = false;
					}
					else
					{
						actualGame.CheckSpecificScreen(num - 1);
						clear_screen();
					}
				}
			}
			else
			{
				cout << "There are no screens in the folder!" << endl;
				cout << "Please put screens in order to run the game..." << endl;
				Sleep(3000);
				clear_screen();
			}
			break;
		case GameManager:: MainMenuOptions::PLAY_FROM_SAVED_SCREEN:
			if (ScreenChekcerDone)
			{
				actualGame.FreeScreens();
				ScreenChekcerDone = false;
			}
			actualGame.getScreens();
			clear_screen();
			numScreens = actualGame.numOfSavedGames();
			if (numScreens != 0)
			{
				actualGame.ScreenChecker();
				ScreenChekcerDone = true;
				clear_screen();
				if (actualGame.printSavedGames())
				{
					cout << "Press 0 for going back to menu...." << endl;
					cin >> num;
					cin.ignore();
					if (num != 0)
					{
						clear_screen();
						if (num > numScreens || num < 0)
						{
							cout << "Wrong input!! going back to main menu..." << endl;
							Sleep(3000);
							clear_screen();
							actualGame.FreeScreens();
							ScreenChekcerDone = false;
						}
						else
						{
							actualGame.CPlayingSavedGame(true);
							actualGame.setSavedGame(num);
							userWantsToPlay = playGame();
						}
					}
				}
			}
			else
			{
				cout << "There are no saved screens in the folder!"<<endl;
				cout<<"Please save screens in order to run a saved game..." << endl;
				Sleep(3000);
				clear_screen();
			}
			break;
		case GameManager::MainMenuOptions::PLAY_FROM_SELECTED_SCREEN:
			if (ScreenChekcerDone)
			{
				actualGame.FreeScreens();
				ScreenChekcerDone = false;
			}
			actualGame.getScreens();
			clear_screen();
			numScreens = actualGame.getNumLevels();
			if (numScreens != 0)
			{
				actualGame.ScreenChecker();
				ScreenChekcerDone = true;
				clear_screen();
				cout << "Select which screen you would like to play:" << endl;
				actualGame.printScreenNames();
				cout << "Press 0 for going back to menu...." << endl;
				cin >> num;
				cin.ignore();
				if (num != 0)
				{
					clear_screen();
					if (num > numScreens || num < 0)
					{
						cout << "Wrong input!! going back to main menu..." << endl;
						Sleep(3000);
						clear_screen();
						actualGame.FreeScreens();
						ScreenChekcerDone = false;
					}
					else
					{
						if (actualGame.isScreenIvalid(num - 1))
						{
							actualGame.setSpecificLevel(num);
							init();
							userWantsToPlay = playGame();
						}

						else
						{
							cout << "The screen you chose is invalid!" << endl;
							cout << "For more information use the screen checker..." << endl;
							Sleep(3000);
							clear_screen();
							actualGame.FreeScreens();
							ScreenChekcerDone = false;
						}
					}
				}
			}
			else
			{
				cout << "There are no screens in the folder!" << endl;
				cout<<"Please put screens in order to run the game..." << endl;
				Sleep(3000);
				clear_screen();
			}
			break;
		case GameManager::MainMenuOptions::EXIT_APPLICATION:
			userWantsToPlay = false;
			break;
		default: // normally we shouldn't get to here...
			userWantsToPlay = false;
		};
	}
}
bool GameManager::playGame()
{
	char action = GameManager::LevelOptions::NEXT_LEVEL;
	while(actualGame.hasNextLevel() && action == GameManager::LevelOptions::NEXT_LEVEL) {
		action = playNextLevel();
	}

	return (action != GameManager::LevelOptions::EXIT_APPLICATION);
}


char GameManager::playNextLevel()
{
	if (!actualGame.IfPlayingSavedGame())
	{
		actualGame.erase();
		actualGame.startLevel();
	}

	bool keepRunning = true;
	bool RunningSol = false;
	char action = 0;
	while(keepRunning)
	{
		//=============================================================================================

		if (!RunningSol)
			action = doLevelIterations();
		else
			RunningSol = false;
		//=============================================================================================
		
		switch(action) {

		case GameManager::LevelOptions::CONTINUE:
			actualGame.printBoard();
			break;
		case GameManager::LevelOptions::REPLAY_LEVEL:
			//--------------------------------------------------------------------------------
			actualGame.FreeScreens();
			actualGame.getScreens();
			ScreenChekcerDone = false;
			actualGame.ScreenChecker();
			actualGame.initLife();
			actualGame.startLevel();
			break;
		case GameManager::LevelOptions::BACK_TO_MAIN_MENU:
			ScreenChekcerDone = false;
			keepRunning = false;
			actualGame.FreeScreens();
			break;
		case GameManager::LevelOptions::EXIT_APPLICATION:
			clear_screen();
			actualGame.FreeScreens();
			ScreenChekcerDone = false;
			keepRunning = false;
			break;
		case GameManager::LevelOptions::SAVE_GAME:
			
			actualGame.saveGame();
			clear_screen();
			cout << "Game Saved!" << endl;
			Sleep(500);
			actualGame.printBoard();
			
			break;
		case GameManager::LevelOptions::SHOW_SOLUTION:
			actualGame.RunSol(true);
			RunningSol = true;
			actualGame.setTime();
			actualGame.getBestMove();
			actualGame.ChangeLevelDone(false);
			actualGame.RunSol(false);
			action = GameManager::LevelOptions::REPLAY_LEVEL;
			break;
		case GameManager::LevelOptions::NEXT_LEVEL:
			actualGame.FreeScreens();
			actualGame.getScreens();
			actualGame.ScreenChecker();
			ScreenChekcerDone = true;
			keepRunning = false;
			break;
		}

	}

	return action;
}

char GameManager::doLevelIterations()
{
	char action;
	bool escapePressed = false;
	//---------------------------------------
	// this is the game iterations loop
	//---------------------------------------
	while (!actualGame.isLevelDone() && !escapePressed && !actualGame.isGameOver() &&  !actualGame.isinValidScreen() && !actualGame.isFinishedScreens()) {


		escapePressed = !doIteration();
	}
	//-----------------------------------------------------
	// END of game iterations loop or exit on ESC
	//-----------------------------------------------------
	if (actualGame.IfPlayingSavedGame())//finished playing saved game
	{
		actualGame.CPlayingSavedGame(false);
		actualGame.FreeSaveScreen();//free memory
	}
	if (actualGame.isFinishedScreens())
	{
		clear_screen();
		actualGame.FinishGame();
		actualGame.CFinishedScreens(false);
		action = GameManager::LevelOptions::BACK_TO_MAIN_MENU;
		clear_screen();
	}
	else if (actualGame.isLevelDone()) {
		clear_screen();
		actualGame.LevelIsDone();
		actualGame.welldone();
		Sleep(250);
		action = GameManager::LevelOptions::NEXT_LEVEL;
	}
	else if (escapePressed) {
		action = 0;
		// print here the sub menu options to the proper place in screen
		clear_screen();
		actualGame.BackToMenu();
		do {
			action = _getch();
		} while (!GameManager::LevelOptions::isValidOption(action));
	}
	else if (actualGame.isinValidScreen())
	{
		action = GameManager::LevelOptions::BACK_TO_MAIN_MENU;
		clear_screen();
	}
	else if (actualGame.isGameOver())
	{
		actualGame.gameover();
		action = GameManager::LevelOptions::BACK_TO_MAIN_MENU;
		clear_screen();
	}
	return action;
}
bool GameManager::doIteration()
{
	unsigned int sleepLength = clockCycleInMillisec/iterationsPerClockCycle;
	for(unsigned int i=0; i<iterationsPerClockCycle-1; ++i) {
		Sleep(sleepLength);
		actualGame.doSubIteration();
	}

	return doInputIteration();
}

bool GameManager::doInputIteration()
{
	list<char> keyHits(KEYBOARD_HIT_LOOP); // set vector initialize size to the max key strokes that we may collect
	bool shouldContinue = true;
	// get keyboard input in a loop, to get enough keyboard hits
	unsigned int sleepLength = clockCycleInMillisec / iterationsPerClockCycle / KEYBOARD_HIT_LOOP;
	for (unsigned int i = 0; i < KEYBOARD_HIT_LOOP; ++i) {
		Sleep(sleepLength);
		if (_kbhit()) {
			char ch = _getch();
			if (ch == ESC) {
				// ESC pressed
				shouldContinue = false;
			}
			else {
				// we got keyHits byref, so we actually set input into the original list sent here
				keyHits.push_front(ch); // we want recent keys to come first, thus we use push_front
			}
		}
		// (even if ESC was hit, we may still have something in the keystrokes vector and should use it)
		actualGame.doIteration(keyHits);

		return shouldContinue;
	}

}