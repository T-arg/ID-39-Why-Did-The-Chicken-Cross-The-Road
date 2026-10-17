#ifndef MENU_H
#define MENU_H


#include "globals.h"
#include "title.h"

void stateMenuIntro()
{
  if (introFrame < INTRO_FRAMES) arduboyIntro(introFrame);   // the ARDUBOY letters land on the notes ...
  else
  {
    sprites.drawSelfMasked(49, 20, T_arg, 0);                // ... then the small T-arg logo
    if (introFrame > INTRO_FRAMES + 52) gameState = STATE_MENU_MAIN;
  }
  introFrame++;
}

void stateMenuMain()
{
  drawTitle();                                  // block letters, road and a hopping chicken (title.h)
  
 
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
