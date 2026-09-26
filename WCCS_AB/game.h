#ifndef GAME_H
#define GAME_H

#include "globals.h"
#include "inputs.h"
#include "playfield.h"

void stateMenuPlay()
{

}

void stateGamePlaying()
{
  drawPlayField();
  drawPlayer();
  checkInputs();
}





void stateGamePause()
{
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    gameState = STATE_GAME_PLAYING;
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
