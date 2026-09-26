#ifndef INPUTS_H
#define INPUTS_H

#include "globals.h"
#include "player.h"


void checkInputs()
{
  if (arduboy.justPressed(UP_BUTTON))
  {
    player.characteristics &= 0B11111100;
    player.characteristics |= 0B01000000;
  }
  if (arduboy.justPressed(LEFT_BUTTON))
  {
    player.characteristics &= 0B11111100;
    player.characteristics |= 0B01000001;
  }
  if (arduboy.justPressed(DOWN_BUTTON))
  {
    player.characteristics &= 0B11111100;
    player.characteristics |= 0B01000010;
    
  }
  if (arduboy.justPressed(RIGHT_BUTTON))
  {
    player.characteristics &= 0B11111100;
    player.characteristics |= 0B01000011; 
  }
   if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    gameState = STATE_GAME_PAUSE;
  }
}


#endif
