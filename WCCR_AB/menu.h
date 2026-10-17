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
  drawTitle();                                  // the moving playfield, the logo and the menu (title.h)

  if (arduboy.justPressed(RIGHT_BUTTON) && (menuSelection < 3)) menuSelection++;
  if (arduboy.justPressed(LEFT_BUTTON) && (menuSelection > 0)) menuSelection--;
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    if      (menuSelection == 0) gameState = STATE_MENU_INFO;      // INFO
    else if (menuSelection == 2) gameState = STATE_MENU_CONF;      // CONF
    else if (menuSelection == 3) gameState = STATE_MENU_SDFX;      // JBOX
    else                                                           // PLAY
    {
      titleReady = false;                       // the title builds a new world next time
      newGame = true;
      gameState = STATE_GAME_PLAYING;
    }
  }
}

// INFO, CONF and JBOX: placeholders until they get a screen of their own (A / B goes back to the title)
void stateMenuConf()
{
  drawSoon(2);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = STATE_MENU_MAIN;
}

void stateMenuInfo()
{
  drawSoon(0);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = STATE_MENU_MAIN;
}

void stateMenuSdfx()
{
  drawSoon(3);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = STATE_MENU_MAIN;
}

#endif
