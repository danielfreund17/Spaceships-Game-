#include <iostream>
#include <windows.h>
#include "SpaceShip.h"
#include "TheShipsGame.h"
#include "GameManager.h"
#include "io_utils.h"
#include <ctime>
using namespace std;

int main() {
	clear_screen();
	hideCursor();
	TheShipsGame game;
	GameManager(game, 200, 2).run();
}