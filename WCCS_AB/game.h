#ifndef GAME_H
#define GAME_H

#include "globals.h"
#include "inputs.h"

void stateMenuPlay()
{
  gameState = 0;
}

void stateGamePlaying()
{
}





void stateGamePause()
{
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
  }
}


void stateGameOver()
{
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {

  }
}

void stateGameFinished()
{

  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) 
  {
  }
}


#endif
