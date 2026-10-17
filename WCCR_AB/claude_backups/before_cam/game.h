#ifndef GAME_H
#define GAME_H

#include "globals.h"
#include "inputs.h"
#include "playfield.h"

// the screen scrolls forward all the time (the world slides down the screen): stand still too long and you are lost
#define SCROLL_LAG   12       // world units the scroll line may trail the chicken
#define LOST_DIST    40       // lost when the camera is this far ahead of the chicken (the chicken is then at the bottom edge)
int16_t scrollY = 0;
byte    scrollSub = 0;        // 1/16 px

void resetGame()
{
  worldInit();
  player.set();
  camX = player.x;
  camY = player.y;
  scrollY = player.y - SCROLL_LAG;
  scrollSub = 0;
}

// camera: glide towards the chicken, always moving at least 1 px
static int16_t camStep(int16_t d)
{
  if (d > 0) return (d + 3) >> 2;
  return -((-d + 3) >> 2);
}

void updateCamera()
{
  if (!(player.characteristics & CH_DEAD) && player.bestRow > 0 && player.hourglass == 0)     // scrolling only starts after the first hop forward
  {
    byte spd = 2 + (player.bestRow >= 40 ? 2 : (player.bestRow >= 20 ? 1 : 0));   // 1/16 px per frame, a little faster later on
    scrollSub += spd;
    scrollY += scrollSub >> 4;
    scrollSub &= 15;
    if (scrollY < player.y - SCROLL_LAG) scrollY = player.y - SCROLL_LAG;     // never far behind a chicken that runs ahead
  }
  int16_t ty = (player.y > scrollY) ? player.y : scrollY;
  camX += camStep(player.x - camX);
  camY += camStep(ty - camY);
}

void stateMenuPlay()
{

}

void stateGamePlaying()
{
  if (newGame)
  {
    resetGame();
    newGame = false;
  }

  checkInputs();
  int16_t cr = camY >> 3;                                    // the camera can be ahead of the chicken now
  if (player.row() > cr) cr = player.row();
  extendWorld(cr);
  updateLanes(cr);
  player.update();
  updateCamera();
  if (!(player.characteristics & CH_DEAD) && (camY - player.y) > LOST_DIST) player.dieLost();

  drawPlayField();
}





void stateGamePause()
{
  drawPlayField();                       // show the frozen scene
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    gameState = STATE_GAME_PLAYING;
  }
}


void stateGameOver()
{
  drawPlayField();                       // show where it went wrong
  if (arduboy.justPressed(A_BUTTON))     // A: back to the title screen
  {
    newGame = true;
    gameState = STATE_MENU_MAIN;
  }
  else if (arduboy.justPressed(B_BUTTON))   // B: play again
  {
    newGame = true;
    gameState = STATE_GAME_PLAYING;
  }
}

void stateGameFinished()
{

  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) 
  {
  }
}


#endif
