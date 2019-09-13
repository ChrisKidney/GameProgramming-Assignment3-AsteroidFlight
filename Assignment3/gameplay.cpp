/*
COMP 369 Assignment 3 - Asteroid Flight
gameplay.cpp
@author: Chris Kidney
Student ID: 3251456
@date: Oct 26th, 2018
*/

#include "asteroidflight.h"

// Sprites
SPRITE theplayer;
SPRITE *player;
SPRITE theasteroids[MAX_ASTEROIDS];
SPRITE *asteroids[MAX_ASTEROIDS];
SPRITE thelaser;
SPRITE *laser;

// Bitmaps for sprite images
BITMAP *buffer;
BITMAP *player_bmp;
BITMAP *asteroid_bmp;
BITMAP *laser_bmp;
BITMAP *explosion_bmp;
BITMAP *destroyed_asteroid_bmp;

// Offset for scrolling
int yoffset = BOTTOM;

// Player's life and score variables
int score = 0;
int life = 3;

/////////////////////////////
// Gameplay Display & Loop //
/////////////////////////////
void gameplayScreen() {

	// Initialize game and load sprites
	initialize();
	loadSprites();

	// Main Gameplay Loop
	while (!key[KEY_ESC]) {

		// Scroll background
		updateScroller(); 

		// Draw and move player
		drawPlayer();
		movePlayer();

		// Draw and move asteroids
		drawAsteroids();
		moveAsteroids();

		// Update laser if fired
		updateLaser();

		// Get input from player
		if (keypressed) {
			getInput();
		}

		// Display score and life
		scoreDisplay();
		lifeDisplay();

		//Tick score up as game progresses
		score++;

		// Blit to double buffer
		acquire_screen();
		blit(buffer, screen, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		release_screen();
	

		// Break out into different menu if gamestate is changed
		if (gameState != 2) {
			// Delete double buffer
			destroy_bitmap(buffer);
			// Delete the Mappy level
			MapFreeMem();
			release_screen();
			break;
		}
	}
}

////////////////////////
// Initialize Program //
////////////////////////
void initialize() {
	set_gfx_mode(MODE, WIDTH, HEIGHT, 0, 0);

	// Create the double buffer and clear it
	buffer = create_bitmap(SCREEN_W, SCREEN_H);
	clear(buffer);

	// Load the Mappy file
	if (MapLoad(MAPFILE) != 0)
	{
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("Can't find level1.fmp");
		return;
	}

	// Set palette
	MapSetPal8();
}

///////////////////////
// Scroll Background //
///////////////////////
void updateScroller()
{
	// Make sure it doesn't scroll beyond map edge
	if (yoffset < 5)
	{
		// End of game
		yoffset = 5;
		rest(1000);
		gameState = 5;
	}

	if (yoffset > BOTTOM) yoffset = BOTTOM;

	// Scroll map up 5 pixels
	yoffset -= 5;

	// Draw map with single layer
	MapDrawBG(buffer, 0, yoffset, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
}

//////////////////
// Load Sprites //
//////////////////
void loadSprites() {
//Configure player's ship
	player = &theplayer;
	player->x = WIDTH/2;
	player->y = HEIGHT/2+175;
	player->xspeed = 0;
	score = 0;
	life = 3;
	player->dir = 3;

	//Load first ship bitmap
	player_bmp = load_bitmap(PLAYERSPRITE, NULL);

// Laser
	//Load laser image
	if (laser_bmp == NULL)
		laser_bmp = load_bitmap(LASERSPRITE, NULL);

	laser = &thelaser;
	laser->x = 0;
	laser->y = 0;
	laser->width = laser_bmp->w;
	laser->height = laser_bmp->h;

// Load initial asteroids
	spawnAsteroids();

	// Load explosion & broken asteroid sprites
	if (explosion_bmp == NULL)
		explosion_bmp = load_bitmap(EXPLOSIONSPRITE, NULL);

	if (destroyed_asteroid_bmp == NULL)
		destroyed_asteroid_bmp = load_bitmap(DESTROYEDASTEROIDSPRITE, NULL);
}

/////////////////
// Draw Player //
/////////////////
void drawPlayer() {
	// Draw player sprite to buffer
	draw_sprite(buffer, player_bmp, player->x, player->y);
}

////////////////////
// Draw Asteroids //
////////////////////
void drawAsteroids() {
	int n;
	
	// Draw asteroid sprites as either alive (full asteroid) or not alive (destroyed asteroid)
	for (n = 0; n < MAX_ASTEROIDS; n++) {
		if (asteroids[n]->alive == 1) {
			draw_sprite(buffer, asteroid_bmp, asteroids[n]->x, asteroids[n]->y);
		}
		else {
			draw_sprite(buffer, destroyed_asteroid_bmp, asteroids[n]->x, asteroids[n]->y);
		}
	}
}

/////////////////
// Move Player //
/////////////////
void movePlayer() {
	// Update player speed
	int speed = player->xspeed;
	player->x += speed;

	// Keep player inside left and right bounds of screen
	if (player->x <= 5) {
		player->xspeed = 0;
		player->x = 5;
	}
	else if (player->x >= WIDTH - 35) {
		player->xspeed = 0;
		player->x = WIDTH - 35;
	}


	// Collision Detection for player ship and asteroids
	int x = player->x;
	int y = player->y;
	int ex[MAX_ASTEROIDS];
	int ey[MAX_ASTEROIDS];

	// Get asteroids x and y values
	int i;
	for (int i = 0; i < MAX_ASTEROIDS; i++) {
		ex[i] = asteroids[i]->x;
		ey[i] = asteroids[i]->y;

	}

	// Check collision against 'alive' asteroids
	int n;
	for (n = 0; n < MAX_ASTEROIDS; n++) {
		if (asteroids[n]->alive) {
			if (inside(x, y, ex[n] - 16, ey[n] - 16, ex[n] + 32, ey[n] + 32))
			{
				// If hit occurs, remove 1 life from player, play explosion animation, spawn new enemy ship
				asteroids[n]->alive = 0;
				life--;
				explode(n);
			}
		}
	}
}

/////////////////////
// Spawn Asteroids //
/////////////////////
void spawnAsteroids() {

	//Load asteroid image
	if (asteroid_bmp == NULL)
		asteroid_bmp = load_bitmap(ASTEROIDSPRITE, NULL);

	int n;
	
	// Spawn MAX_ASTEROIDS worth of asteroids at random locations along x, and just above the playable screen on y
	for (n = 0; n < MAX_ASTEROIDS; n++) {
		asteroids[n] = &theasteroids[n];
		asteroids[n]->alive = 1;
		asteroids[n]->x = rand() % (620 - 30 + 1) + 1; 
		asteroids[n]->y = -(rand() % ((300) - (100) + 1) + 1);
		asteroids[n]->width = asteroid_bmp->w;
		asteroids[n]->height = asteroid_bmp->h;
	}
}

////////////////////
// Move Asteroids //
////////////////////
void moveAsteroids() {
	int n;
	// Move all asteroids down the screen
	for (n = 0; n < MAX_ASTEROIDS; n++) {
		asteroids[n]->xspeed++;
		if (asteroids[n]->xspeed > 4)
			asteroids[n]->xspeed = 4;

		asteroids[n]->y += asteroids[n]->xspeed;

		// Once asteroids pass the bottom edge of the screen, spawn a new wave of them
		if (asteroids[n]->y > HEIGHT + 200) {
			spawnAsteroids();
		}
	}
	

}

///////////////
// Get Input //
///////////////
void getInput() {
	static bool chk = true;

	//AD + SPACE keys control player ship
	if (key[KEY_A]) strafeLeft();
	if (key[KEY_D]) strafeRight();

	if (key[KEY_SPACE]) fireLaser();

	// ESC to quit game
	if (key[KEY_ESC]) allegro_exit();

	// Mute / Play music control
	if (key[KEY_M]) {
		if (key_shifts & KB_CTRL_FLAG) {
			chk = !chk;
			if (chk == false) {
				midi_pause();
			}
			else {
				midi_resume();
			}
		}
	}

	// Go to Help Menu if CTRL+H is pressed
	if (key[KEY_H]) {
		if (key_shifts & KB_CTRL_FLAG) {
			gameState = 3;
		}
	}

	rest(20);
}

//////////////////////////////////
// Strafe Left & Right Controls //
//////////////////////////////////
void strafeLeft() {
	// Move ship left
	player->xspeed--;
	if (player->xspeed <= -3)
		player->xspeed = -3;

}
void strafeRight() {
	// Move ship right
	player->xspeed++;
	if (player->xspeed > 3)
		player->xspeed = 3;
}

////////////////
// Fire Laser //
////////////////
void fireLaser()
{
	// Load sound effects
	SAMPLE *lasersound;

	if (install_sound(DIGI_AUTODETECT, MIDI_NONE, "") != 0) {
		allegro_message("Error initializing sound system.");
	}
	lasersound = load_sample(LASERSOUND);
	if (!lasersound) {
		allegro_message("Error reading .wav file");
	}

	int x = player->x;
	int y = player->y;

	// Check if laser is ready to fire
	if (!laser->alive)
	{
		laser->alive = 1;
		play_sample(lasersound, 255, 128, 1000, FALSE); // Play laser sound effect

	    // Fire laser 
		laser->x = x - 2;
		laser->y = y - 22;
		laser->xspeed = 0;
		laser->yspeed = -LASERSPEED;		
	}
}

////////////////
//Update Laser//
////////////////
void updateLaser()
{
	int x, y;
	int ex[MAX_ASTEROIDS], ey[MAX_ASTEROIDS];

	x = laser->x;
	y = laser->y;

	if (!laser->alive) return;

	rectfill(screen, x, y, x + 10, y + 10, BLACK);

	// Move laser
	laser->x += laser->xspeed;
	laser->y += laser->yspeed;
	x = laser->x;
	y = laser->y;

	// Keep laser on screen
	if (x < 6 || x > SCREEN_W - 6 || y < 20 || y > SCREEN_H - 6)
	{
		laser->alive = 0;
		return;
	}

	//Hit detection	
	int n;

	for (n = 0; n < MAX_ASTEROIDS; n++) {
		ex[n] = asteroids[n]->x;
		ey[n] = asteroids[n]->y;
		if (asteroids[n]->alive) {
			if (inside(x, y, ex[n] - 16, ey[n] - 16, ex[n] + 16, ey[n] + 16))
			{
				// If hit occurs, 'kill' the laser, explode enemy ship
					laser->alive = 0;
					asteroids[n]->alive = 0;
					score+= 100; // Increase score
					explode(n); // Blow up asteroid
			}
			else
				// If no hit then draw laser
			{
				draw_sprite(screen, laser_bmp, x, y);
			}
		}
	}
}

/////////////////
//Score Display//
/////////////////
void scoreDisplay() {
	// Display score during gameplay
	std::string conv = std::to_string(score);
	const char * con = conv.c_str();

	textprintf_centre_ex(buffer, font, SCREEN_W - 65, 5, WHITE, -1, "Score:");
	textprintf_centre_ex(buffer, font, SCREEN_W - 20, 5, WHITE, -1, con);
}

/////////////////
//Life Display//
/////////////////
void lifeDisplay() {
	// Display life during gameplay
	std::string conv = std::to_string(life);
	const char * con = conv.c_str();

	textprintf_centre_ex(buffer, font, SCREEN_W - 600, 5, WHITE, -1, "Lives:");
	textprintf_centre_ex(buffer, font, SCREEN_W - 565, 5, WHITE, -1, con);

	// If lives go below 0, player loses, go to game over
	if (life < 0) {
		gameState = 4;
	}
}

////////////////////////
//Explosion Controller//
////////////////////////
void explode(int num)
{
	// Load sound effects
	SAMPLE *explosion;

	if (install_sound(DIGI_AUTODETECT, MIDI_NONE, "") != 0) {
		allegro_message("Error initializing sound system.");
	}
	explosion = load_sample(EXPLOSIONSOUND);
	if (!explosion) {
		allegro_message("Error reading .wav file");
	}

	// Play explosion sound effect
	play_sample(explosion, 255, 128, 1000, FALSE);

	int n;

	int x = asteroids[num]->x;
	int y = asteroids[num]->y;


	// Draw explosion animation
	for (n = 0; n < 100; n++)
	{
		rotate_sprite(buffer, explosion_bmp,
			x + rand() % 10 - 20, y + rand() % 10 - 20,
			itofix(rand() % 255));
	}
}