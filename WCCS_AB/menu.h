#ifndef MENU_H
#define MENU_H


#include "globals.h"


void stateMenuIntro()
{
  sprites.drawSelfMasked(49, 20, T_arg, 0);
}

void stateMenuMain()
{
 
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

    gameState = menuSelection + 1;

    
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
