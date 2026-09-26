#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <Arduboy2.h>
#include <Sprites.h>
#include "bitmaps.h"


// constants /////////////////////////////////////////////////////////////////

//define menu states (on main menu)

#define STATE_MENU_MAIN              0

#define STATE_MENU_CONF              1
#define STATE_MENU_SDFX              2
#define STATE_MENU_INFO              3
#define STATE_MENU_PLAY              4

#define STATE_MENU_INTRO             5


//define game states (on main menu)
#define STATE_GAME_PLAYING           6
#define STATE_GAME_PAUSE             9
#define STATE_GAME_SPLASH            10
#define STATE_GAME_OVER              11

//define facing directions
#define EAST                          0
#define NORTH                         1
#define WEST                          2
#define SOUTH                         3


// globals ///////////////////////////////////////////////////////////////////

Arduboy2Base arduboy;
Sprites sprites;

byte gameState = STATE_MENU_INTRO;   // start the game with the intro logo
byte menuSelection = STATE_MENU_PLAY; // PLAY menu item is pre-selected

byte test = 0;


#endif
