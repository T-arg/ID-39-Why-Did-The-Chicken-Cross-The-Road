#ifndef MENU_H
#define MENU_H


#include "globals.h"


void stateMenuIntro()
{
  if (arduboy.everyXFrames(30)) gameState = STATE_MENU_MAIN;
  sprites.drawSelfMasked(49, 20, T_arg, 0);
}

void stateMenuMain()
{
    arduboy.fillRect(0, 0, 128, 64, WHITE);
  sprites.drawErase(18, 4, titleChicken, 0);
  
 
  if (arduboy.justPressed(RIGHT_BUTTON) && (menuSelection < 3))
  {

    menuSelection++;
  }
  if (arduboy.justPressed(LEFT_BUTTON) && (menuSelection > 0))
  {

    menuSelection--;
  }
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {

    gameState = STATE_GAME_PLAYING;

    
  }
}

void stateMenuConf()
{
  
}

void stateMenuInfo()
{

  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {

  }
}

void stateMenuSdfx()
{
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {

    menuSelection = STATE_MENU_SDFX;

  }
}

#endif
