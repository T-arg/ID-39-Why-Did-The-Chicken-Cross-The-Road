#ifndef PLAYER_H
#define PLAYER_H

#include "globals.h"

#define PLAYER_X              54
#define PLAYER_Y              20

const unsigned char PROGMEM playerJumpPattern[] =
{
  0,1,1,2,1,1,0,0,0,1,1,2,1,1,
};

const unsigned char PROGMEM playerY[] =
{
  0,0,0,0,0,1,2,3,2,1,0,0,0,0,
};


struct chickenRunner
{
  public:
    int x, y;
    byte characteristics;
    byte jumpCounter;

    void set()
    {
      x = PLAYER_X;
      y = PLAYER_Y;
      characteristics = 0b00010000;
      //                  ||||||||
    //                    |||||||└->  0  Chicken Direction 0 = EAST ; 1 = NORTH ; 2 = WEST ; 3 = SOUTH
    //                    ||||||└-->  1  Chicken Direction
    //                    |||||└--->  2  
    //                    ||||└---->  3  
    //                    |||└----->  4  Chicken Visible
    //                    ||└------>  5  Chicken Splat
    //                    |└------->  6  Chicken Jumping
    //                    └-------->  7  Chicken Dead
    jumpCounter = 0;
    }
};

chickenRunner player;

void drawPlayer()
{
  Serial.println(player.characteristics, BIN);
  if ((player.characteristics & 0b11100000) != 0)
  {
    if (arduboy.everyXFrames(2))
    {
      player.jumpCounter++;
    }
    if (player.jumpCounter>13)
    {
      player.jumpCounter = 0;
      player.characteristics &= 0B10111111;
    }
  }
  byte facing = pgm_read_byte(&playerJumpPattern[player.jumpCounter])  + (3*(player.characteristics & 0B00000011));
  sprites.drawPlusMask(54, 20-pgm_read_byte(&playerY[player.jumpCounter]), Chicken_plus_mask, facing);
}

#endif