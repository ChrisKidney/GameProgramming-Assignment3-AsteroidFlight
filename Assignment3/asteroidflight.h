/*
COMP 369 Assignment 3 - Asteroid Flight
asteroidflight.h
@author: Chris Kidney
Student ID: 3251456
@date: Oct 26th, 2018
*/
#pragma once
#ifndef _ASTEROIDFLIGHT_H
#define _ASTEROIDFLIGHT_H

#include <string>
#include <allegro.h>
#include "mappyal.h"oad t

extern int gameState; 

extern int score;
extern int life;


// Allegro Graphics Constants
#define MODE GFX_AUTODETECT_WINDOWED
#define WIDTH 640
#define HEIGHT 480
#define COLORDEPTH 16

// Menu images
#define MAINMENU "mainmenu.bmp"
#define HELPMENU "helpscreen.bmp"
#define CONGRATSMENU "congratsscreen.bmp"
#define GAMEOVER "gameoverscreen.bmp"

// Sprite images
#define PLAYERSPRITE "player.bmp"
#define ASTEROIDSPRITE "asteroid.bmp"
#define DESTROYEDASTEROIDSPRITE "desasteroid.bmp"
#define EXPLOSIONSPRITE "explosion.bmp"
#define LASERSPRITE "laser.bmp"

// Map & Gameplay Constants
#define MAPFILE "level01.FMP"
#define BOTTOM 24000 - HEIGHT
#define MAX_ASTEROIDS 8
#define LASERSPEED 13

// Sound files
#define BACKGROUND "background.mid"
#define LASERSOUND "laser.wav"
#define EXPLOSIONSOUND "explosion.wav"
#define HOVER "hover.wav"
#define CLICK "click.wav"

// Colors
#define BLACK makecol(0,0,0)
#define WHITE makecol(255,255,255)
#define YELLOW makecol(255,255,100)


// Sprite struct
typedef struct SPRITE {
	int dir, alive;
	int x, y;
	int width, height;
	int xspeed, yspeed;
}SPRITE;


// Function Prototypes
// Menus
void mainMenu();
void helpMenu();
void gameplayScreen();
void gameoverScreen();
void congratsScreen();

// Utilities
void musicControl();
int mousePos(int x1, int y1, int x2, int y2);
int inside(int x, int y, int left, int top, int right, int bottom);

// Gameplay
void initialize();
void updateScroller();
void loadSprites();
void drawPlayer();
void movePlayer();
void getInput();
void strafeLeft();
void strafeRight();
void fireLaser();
void updateLaser();
void drawAsteroids();
void moveAsteroids();
void spawnAsteroids();
void scoreDisplay();
void lifeDisplay();
void explode(int num);
#endif