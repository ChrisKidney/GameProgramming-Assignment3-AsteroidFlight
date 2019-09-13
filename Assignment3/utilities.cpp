/*
COMP 369 Assignment 3 - Asteroid Flight
utilties.cpp
@author: Chris Kidney
Student ID: 3251456
@date: Oct 26th, 2018
*/

#include "asteroidflight.h"

//////////////////////////////
// Background Music Control //
//////////////////////////////
void musicControl() {
	MIDI *music;
	if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) != 0) {
		allegro_message("Error initializing sound system\n%s\n", allegro_error);
	}
	//load the midi file
	music = load_midi(BACKGROUND);
	set_volume(235, 145);

	if (!music) {
		allegro_message("Error loading Midi file");
	}

	if (play_midi(music, 0) != 0) {
		allegro_message("Error playing Midi\n%s", allegro_error);

	}
}

/////////////////////////////
// Mouse Position Function //
/////////////////////////////
int mousePos(int x1, int y1, int x2, int y2)
{
	if (mouse_x > x1 && mouse_x < x2 && mouse_y > y1 && mouse_y < y2)
		return 1;
	else
		return 0;
}

///////////////////
//Inside Function//
///////////////////
int inside(int x, int y, int left, int top, int right, int bottom)
{
	if (x > left && x < right && y > top && y < bottom)
		return 1;
	else
		return 0;
}