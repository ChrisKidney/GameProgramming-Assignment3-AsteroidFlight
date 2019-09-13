/*
COMP 369 Assignment 3 - Asteroid Flight
@author: Chris Kidney
Student ID: 3251456
@date: Oct 26th, 2018
*/

#include "asteroidflight.h"

///////////////
// Main Menu //
///////////////
void mainMenu() {
	// Set up Bitmap
	char *filename = MAINMENU;
	BITMAP *image;
	int ret;
	set_color_depth(COLORDEPTH);
	ret = set_gfx_mode(MODE, WIDTH, HEIGHT, 0, 0);
	if (ret != 0) {
		allegro_message(allegro_error);
	}

	// Load image
	image = load_bitmap(filename, NULL);
	if (!image) {
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("Error loading %s", filename);
	}

	// Load sound effects
	SAMPLE *hover;
	SAMPLE *click;
	if (install_sound(DIGI_AUTODETECT, MIDI_NONE, "") != 0) {
		allegro_message("Error initializing sound system.");
	}
	hover = load_sample(HOVER);
	if (!hover) {
		allegro_message("Error reading .wav file");
	}
	click = load_sample(CLICK);
	if (!click) {
		allegro_message("Error reading .wav file");
	}

	// Draw background image
	blit(image, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
	destroy_bitmap(image);

	// Draw text to screen
	textprintf_centre_ex(screen, font, SCREEN_W / 2, SCREEN_H / 2 - 50, WHITE, -1, "Play Game!");
	textprintf_centre_ex(screen, font, SCREEN_W / 2, SCREEN_H / 2 - 25, WHITE, -1, "How to Play!");
	textprintf_centre_ex(screen, font, SCREEN_W / 2, SCREEN_H / 2 + 25, WHITE, -1, "Press ESC at any time to quit.");

	// Boolean used to create hover effect on selections
	bool check = false;

	//Show mouse cursor
	show_mouse(screen);

	while (!key[KEY_ESC]) {
		if (mousePos(250, 170, 400, 200) && (check == false)) {
			// Play sound when mouse hovers over selection
			play_sample(hover, 255, 128, 1000, FALSE);
			check = true;
		}
		// Changes text color to highlight that the user is moused over an option, else if is used with boolean to prevent sound clip from infinitely looping if the mouse stays over selectable area
		else if (mousePos(220, 170, 400, 200) && (check == true)) {
			// Change text color when mouse hovers over selection
			textprintf_centre_ex(screen, font, SCREEN_W / 2, SCREEN_H / 2 - 50, YELLOW, -1, "Play Game!");
			// If left mouse button is pressed, move to next screen and play a click sound effect
			if (mouse_b & 1) {
				play_sample(click, 255, 128, 1000, FALSE);
				gameState = 2;
				break;
			}
		}
		else if (mousePos(250, 200, 400, 230) && (check == false)) {
			// Play sound when mouse hovers over selection
			play_sample(hover, 255, 128, 1000, FALSE);
			check = true;
		}
		// Changes text color to highlight that the user is moused over an option, else if is used with boolean to prevent sound clip from infinitely looping if the mouse stays over selectable area
		else if (mousePos(250, 200, 400, 230) && (check == true)) {
			// Change text color when mouse hovers over selection
			textprintf_centre_ex(screen, font, SCREEN_W / 2, SCREEN_H / 2 - 25, YELLOW, -1, "How to Play!");
			// If left mouse button is pressed, move to next screen and play a click sound effect
			if (mouse_b & 1) {
				play_sample(click, 255, 128, 1000, FALSE);
				gameState = 3;
				break;
			}
		}
		else {
			textprintf_centre_ex(screen, font, SCREEN_W / 2, SCREEN_H / 2 - 50, WHITE, -1, "Play Game!");
			textprintf_centre_ex(screen, font, SCREEN_W / 2, SCREEN_H / 2 - 25, WHITE, -1, "How to Play!");
		}
	}
}

///////////////
// Help Menu //
///////////////
void helpMenu() {
	// Set up Bitmap
	char *filename = HELPMENU;
	BITMAP *image;
	int ret;
	set_color_depth(COLORDEPTH);
	ret = set_gfx_mode(MODE, WIDTH, HEIGHT, 0, 0);
	if (ret != 0) {
		allegro_message(allegro_error);
	}

	// Load image
	image = load_bitmap(filename, NULL);
	if (!image) {
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("Error loading %s", filename);
	}

	// Draw background image
	blit(image, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
	destroy_bitmap(image);

	//Show mouse cursor
	show_mouse(screen);

	while (!key[KEY_ESC]) {
		if (key[KEY_BACKSPACE]) {
			gameState = 1;
			break;
		}
		if (key[KEY_H]) {
			if (key_shifts & KB_CTRL_FLAG) {
				gameState = 2;
				break;
			}
		}
		if (mouse_b & 1) {
			gameState = 1;
			break;
		}
	}
}

/////////////////////
// Gameover Screen //
/////////////////////
void gameoverScreen() {
	// Set up Bitmap
	char *filename = GAMEOVER;
	BITMAP *image;
	int ret;
	set_color_depth(COLORDEPTH);
	ret = set_gfx_mode(MODE, WIDTH, HEIGHT, 0, 0);
	if (ret != 0) {
		allegro_message(allegro_error);
	}

	// Load image
	image = load_bitmap(filename, NULL);
	if (!image) {
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("Error loading %s", filename);
	}

	// Draw background image
	blit(image, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
	destroy_bitmap(image);

	//Show mouse cursor
	show_mouse(screen);

	// Display final score
	std::string conv = std::to_string(score);
	const char * con = conv.c_str();
	textprintf_centre_ex(screen, font, SCREEN_W / 2 - 50, SCREEN_H - 300, WHITE, -1, "Final Score: ");
	textprintf_centre_ex(screen, font, SCREEN_W / 2 + 50, SCREEN_H - 300, WHITE, -1, con);

	while (!key[KEY_ESC]) {
		if (key[KEY_BACKSPACE]) {
			gameState = 1;
			break;
		}
		if (mouse_b & 1) {
			gameState = 1;
			break;
		}
	}
}

/////////////////////
// Congrats Screen //
/////////////////////
void congratsScreen() {
	// Set up Bitmap
	char *filename = CONGRATSMENU;
	BITMAP *image;
	int ret;
	set_color_depth(COLORDEPTH);
	ret = set_gfx_mode(MODE, WIDTH, HEIGHT, 0, 0);
	if (ret != 0) {
		allegro_message(allegro_error);
	}

	// Load image
	image = load_bitmap(filename, NULL);
	if (!image) {
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("Error loading %s", filename);
	}

	// Draw background image
	blit(image, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
	destroy_bitmap(image);

	//Show mouse cursor
	show_mouse(screen);

	// Display final score
	std::string conv = std::to_string(score);
	const char * con = conv.c_str();
	textprintf_centre_ex(screen, font, SCREEN_W / 2 - 50, SCREEN_H - 300, WHITE, -1, "Final Score: ");
	textprintf_centre_ex(screen, font, SCREEN_W / 2 + 50, SCREEN_H - 300, WHITE, -1, con);

	while (!key[KEY_ESC]) {
		if (key[KEY_BACKSPACE]) {
			gameState = 1;
			break;
		}
		if (mouse_b & 1) {
			gameState = 1;
			break;
		}
	}
}