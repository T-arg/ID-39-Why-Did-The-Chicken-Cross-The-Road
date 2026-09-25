#ifndef GAME_H
#define GAME_H

#include "globals.h"
#include "inputs.h"

byte test = 0;
bool jump = false;
byte direction = 0;

void stateMenuPlay()
{
    if (arduboy.justPressed(UP_BUTTON))
  {
    jump = true;
    direction = EAST;
  }
     if (arduboy.justPressed(LEFT_BUTTON))
  {
    jump = true;
    direction = NORTH;
  }
      if (arduboy.justPressed(DOWN_BUTTON))
  {
    jump = true;
    direction = WEST;
  }
        if (arduboy.justPressed(RIGHT_BUTTON))
  {
    jump = true;
    direction = SOUTH;
  }
  if (jump == true)
  {
    if (arduboy.everyXFrames(2))
    {
      test++;
    }
    if (test>13)
    {
      test = 0;
      jump = false;
    }
  }
  byte facing = chicken[test] + (3*direction);
  sprites.drawPlusMask(54, 20-chickenY[test], Chicken_plus_mask, facing);
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
