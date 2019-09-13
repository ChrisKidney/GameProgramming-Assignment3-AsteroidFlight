/*
COMP 369 Assignment 3 - Asteroid Flight
main.cpp
@author: Chris Kidney
Student ID: 3251456
@date: Oct 26th, 2018
Program Description: Program is a spaceship shooting game. Player controls the white and blue ship and trys to dodge incoming enemy ship while firing lasers to destroy enemy ships.
*/
#include "asteroidflight.h"

int gameState = 1;	// Global game state variable, used to control program flow through screens

//////////
// Main //
//////////
int main()
{
	// Initialize allegro
	allegro_init();
	set_color_depth(COLORDEPTH);
	set_window_title("Assignment 3 - Asteroid Flight");
	install_keyboard();
	install_timer();
	install_mouse();
	srand(time(NULL));

	musicControl(); // Start background music

	// Main Game Loop
	while (!key[KEY_ESC]) {
		if (gameState == 1) {
			mainMenu();
		}
		if (gameState == 2) {
			gameplayScreen();
		}
		if (gameState == 3) {
			helpMenu();
		}
		if (gameState == 4) {
			gameoverScreen();
		}
		if (gameState == 5) {
			congratsScreen();
		}
	}

	allegro_exit();
	return 0;
}
END_OF_MAIN()