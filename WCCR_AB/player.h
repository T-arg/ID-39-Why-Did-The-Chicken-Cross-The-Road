#ifndef PLAYER_H
#define PLAYER_H

#include "globals.h"
#include "world.h"

// hop animation speed: steps advanced per frame (the hop is 14 steps). 1 = 14 frames, 2 = 7 frames.
// HOP_EXTRA adds one extra step every 2nd frame: 1 step/frame + extra = ~9 frames per hop.
#define HOP_STEPS             1
#define HOP_EXTRA             1

// where the centre of the chicken's feet (the base ellipse) is inside the 17x24 sprite;
// that point is put on the centre of the cell. Tune these to move the chicken on the map.
#define CHICKEN_FOOT_X        8
#define CHICKEN_FOOT_Y        21

#define SPLASH_DROPS          30           // drops in the splash

// bits of player.characteristics
#define CH_DIR_MASK           0b00000011   // hop / facing direction: 0 = UP, 1 = LEFT, 2 = DOWN, 3 = RIGHT
#define CH_VISIBLE            0b00010000
#define CH_SPLAT              0b00100000   // hit by a vehicle
#define CH_JUMPING            0b01000000
#define CH_DEAD               0b10000000
#define CH_FEATHERS           0b00000100   // hit by a plane: burst of feathers
#define CH_LOST               0b00001000   // the scrolling screen left the chicken behind

#define HOP_UP                0            // forward  : +Y, screen up-right
#define HOP_LEFT              1            // along    : -X, screen up-left
#define HOP_DOWN              2            // backward : -Y, screen down-left
#define HOP_RIGHT             3            // along    : +X, screen down-right

const unsigned char PROGMEM playerJumpPattern[] =
{
  0,1,1,2,1,1,0,0,0,1,1,2,1,1,
};

const unsigned char PROGMEM playerY[] =
{
  0,0,0,0,0,1,2,3,2,1,0,0,0,0,
};

// how far along the hop (0..16) the chicken is at each animation step
const unsigned char PROGMEM playerHopProgress[] =
{
  0,0,0,0,1,3,6,10,13,15,16,16,16,16,
};

// falling from the sky (at the start and after a golden egg): height in px for every frame, with one bounce at the end
const unsigned char PROGMEM dropH[] =
{
  58, 58, 57, 55, 53, 49, 46, 41, 36, 30, 24, 16, 9, 0, 3, 5, 7, 8, 9, 9, 9, 8, 7, 5, 3, 0, 0,
};
#define DROP_LEN              27

const int8_t PROGMEM hopDX[] = { 0, -8,  0,  8 };
const int8_t PROGMEM hopDY[] = { 8,  0, -8,  0 };


// logs and leaves float above the water: a chicken standing on one sits this many px higher
#define LOG_LIFT              3

// how many px a chicken at world X = x in row v is lifted: on a log 3, on a sinking leaf 3 - stage, else 0
int8_t liftAt(int16_t v, int16_t x)
{
  const Lane &L = laneAt(v);
  if (L.type != LANE_WATER) return 0;
  if (isLeafLane(L))
  {
    int16_t u = x >> 3;
    if (!leafAt(L, u)) return 0;
    uint8_t st = leafStage(v, u);
    return (st >= 4) ? 0 : LOG_LIFT - st;
  }
  return logUnder(L, x) ? LOG_LIFT : 0;
}

struct chickenRunner
{
  public:
    int16_t x, y;                 // world position (px) of the chicken's cell centre
    int16_t fromX, fromY, toX, toY;
    byte characteristics;
    byte jumpCounter;
    byte queued;                  // 0 = nothing, else 1 + direction of a buffered hop
    byte deadTimer;
    byte splashSeed;              // random seed of the splash
    int16_t bestRow;              // furthest row reached
    uint16_t score;               // 1 point per new row reached
    int8_t  lift, fromLift, toLift;   // px the chicken is raised (standing on a log / leaf)
    byte     eggs;                // golden eggs = extra lives
    byte     invuln;              // frames of protection after an egg saved the chicken
    byte     dropT;               // > 0 while the chicken falls out of the sky (DROP_LEN .. 1)
    uint16_t hourglass;           // frames the scrolling is stopped
    uint16_t magnet;              // frames the magnet pulls in corn

    void set()
    {
      x = 4;                      // centre of cell u = 0
      y = 4;                      // centre of row v = 0
      characteristics = CH_VISIBLE | HOP_UP;
      //                  ||||||||
    //                    |||||||└->  0  Chicken Direction 0 = UP ; 1 = LEFT ; 2 = DOWN ; 3 = RIGHT
    //                    ||||||└-->  1  Chicken Direction
    //                    |||||└--->  2
    //                    ||||└---->  3
    //                    |||└----->  4  Chicken Visible
    //                    ||└------>  5  Chicken Splat
    //                    |└------->  6  Chicken Jumping
    //                    └-------->  7  Chicken Dead
      jumpCounter = 0;
      queued = 0;
      deadTimer = 0;
      bestRow = 0;
      score = 0;
      lift = fromLift = toLift = 0;
      eggs = 0; invuln = 0; hourglass = 0; magnet = 0; dropT = DROP_LEN;
    }

    int16_t row() { return y >> 3; }

    // the row the chicken is DRAWN in. While it hops from one lane to the next it
    // belongs to the nearer (lower) lane, so it ends up in front of the far lane's
    // objects and behind the near lane's objects.
    int16_t drawRow()
    {
      if (characteristics & CH_JUMPING)
      {
        int16_t a = fromY >> 3, b = toY >> 3;
        return (a < b) ? a : b;
      }
      return y >> 3;
    }

    // an egg saves the chicken: it is set down on the nearest free grass cell (ahead first), safe for a moment
    bool tryEgg(bool lost)
    {
      if (!eggs || (characteristics & CH_DEAD)) return false;
      int16_t v0 = lost ? deadRow() + 2 : row() + 1;
      int16_t col0 = x >> 3;
      if (col0 < PLAY_MIN_U) col0 = PLAY_MIN_U;
      if (col0 > PLAY_MAX_U) col0 = PLAY_MAX_U;
      for (byte k = 0; k < 10; k++)
      {
        int16_t v = v0 + ((k < 7) ? (int16_t)k : 6 - (int16_t)k);       // 7 rows ahead, then 3 back
        if (v < deadRow() || laneAt(v).type != LANE_GRASS) continue;
        for (byte d = 0; d <= (PLAY_MAX_U - PLAY_MIN_U); d++)
        {
          for (int8_t sgn = -1; sgn <= 1; sgn += 2)
          {
            int16_t c = col0 + sgn * (int16_t)d;
            if (c < PLAY_MIN_U || c > PLAY_MAX_U || bushAt(v, c)) continue;
            eggs--;
            x = fromX = toX = c * 8 + 4;
            y = fromY = toY = v * 8 + 4;
            lift = fromLift = toLift = 0;
            characteristics &= ~(CH_JUMPING | CH_LOST);
            characteristics |= CH_VISIBLE;
            jumpCounter = 0; queued = 0;
            invuln = 60;
            dropT = DROP_LEN;                                            // falls out of the sky again
            if (row() > bestRow) bestRow = row();
            return true;
          }
        }
      }
      return false;
    }

    void die(bool splat)
    {
      if (tryEgg(false)) return;
      characteristics &= ~CH_JUMPING;
      characteristics |= CH_DEAD | (splat ? CH_SPLAT : 0);
      splashSeed = rnd() | 1;
      if (splat)
      {
        if (laneAt(y >> 3).type == LANE_RUNWAY && laneAt(y >> 3).kind == 1) y -= 8;      // far lane of a runway: stick to the near side of the plane
        y = ((y >> 3) << 3) + 1;                       // squash it against the side of the vehicle (its near face)
        jumpCounter = 0;
      }
      if (!splat) characteristics &= ~CH_VISIBLE;      // fell in the water: gone, a splash takes over
      queued = 0;
      deadTimer = 0;
      lift = 0;                                        // splash / splat happen at floor level
    }

    void dieFeathers()                                 // hit by a plane: the chicken bursts into feathers
    {
      if (tryEgg(false)) return;
      characteristics &= ~(CH_JUMPING | CH_VISIBLE);
      characteristics |= CH_DEAD | CH_FEATHERS;
      splashSeed = rnd() | 1;
      queued = 0; deadTimer = 0; jumpCounter = 0;
    }

    void dieLost()                                     // the screen scrolled past the chicken
    {
      if (tryEgg(true)) return;
      characteristics &= ~CH_JUMPING;
      characteristics |= CH_DEAD | CH_LOST;
      queued = 0;
      deadTimer = 0;
      lift = 0;
    }

    void startHop(byte dir)
    {
      characteristics = (characteristics & ~CH_DIR_MASK) | dir;      // always turn

      int16_t tx = x + (int8_t)pgm_read_byte(&hopDX[dir]);
      int16_t ty = y + (int8_t)pgm_read_byte(&hopDY[dir]);
      if ((dir == HOP_UP) || (dir == HOP_DOWN)) tx = ((x >> 3) << 3) + 4;   // snap to the grid when changing lane

      int16_t tu = tx >> 3, tv = ty >> 3;
      if (tu < PLAY_MIN_U || tu > PLAY_MAX_U) return;                // edge of the field
      if (tv < bestRow - 12) return;                                 // (the black dead zone ends the walk back long before this)
      if (bushAt(tv, tu)) return;                                    // bush in the way

      fromX = x; fromY = y; toX = tx; toY = ty;
      fromLift = liftAt(y >> 3, x); toLift = liftAt(ty >> 3, tx);
      jumpCounter = 0;
      characteristics |= CH_JUMPING;
      sfx(sfxHop);
    }

    void queueHop(byte dir)
    {
      if ((characteristics & CH_DEAD) || dropT) return;               // no hopping while falling
      if (characteristics & CH_JUMPING) queued = dir + 1;
      else startHop(dir);
    }

    void takeItem(Lane &L)
    {
      byte t = itemType(L);
      L.item = 0;
      if (t == ITEM_CORN)           { score = (score < 65000) ? score + 5 : score; sfx(sfxCoin); }
      else if (t == ITEM_EGG)       { if (eggs < MAX_EGGS) eggs++; else if (score < 65000) score += 10; }
      else if (t == ITEM_HOURGLASS) { hourglass = HOURGLASS_TIME; }
      else if (t == ITEM_MAGNET)    { magnet = MAGNET_TIME; }
    }

    void land()
    {
      x = toX; y = toY;
      characteristics &= ~CH_JUMPING;
      jumpCounter = 0;
      if (row() > bestRow) { bestRow = row(); if (score < 65535) score++; }   // a point for every new row reached
      { Lane &PL = laneAt(row()); if (itemType(PL) && itemU(PL) == (x >> 3)) takeItem(PL); }   // pick up what is on this cell
      if (queued)
      {
        byte q = queued - 1;
        queued = 0;
        startHop(q);
      }
    }

    void update()
    {
      if (characteristics & CH_DEAD)
      {
        deadTimer++;
        if (characteristics & CH_SPLAT) x += laneAt(row()).dx;                             // stuck to the vehicle, moves along with it
        if (deadTimer > ((characteristics & CH_LOST) ? 12 : 40)) { gameState = STATE_GAME_OVER; }
        return;
      }

      if (invuln) invuln--;
      if (dropT)
      {
        dropT--;
        byte i = DROP_LEN - dropT;
        if (dropT == 0) sfx(sfxHop);                                   // touch down
      }
      if (hourglass) hourglass--;
      if (magnet)
      {
        magnet--;
        if (arduboy.everyXFrames(3))                                  // pull in one corn at a time from the cells around
        {
          for (int16_t v = row() - 1; v <= row() + 4; v++)
          {
            Lane &ML = laneAt(v);
            int16_t du = itemU(ML) - (x >> 3);
            if (itemType(ML) == ITEM_CORN && du >= -4 && du <= 4) { takeItem(ML); break; }
          }
        }
      }

      if (characteristics & CH_JUMPING)
      {
        byte steps = HOP_STEPS + ((HOP_EXTRA && arduboy.everyXFrames(2)) ? 1 : 0);
        while (steps-- && (characteristics & CH_JUMPING))
        {
          jumpCounter++;
          if (jumpCounter > 13) land();
        }
        if (characteristics & CH_JUMPING)
        {
          byte p = pgm_read_byte(&playerHopProgress[jumpCounter]);
          x = fromX + (((toX - fromX) * p) >> 4);
          y = fromY + (((toY - fromY) * p) >> 4);
          lift = fromLift + (((toLift - fromLift) * (int8_t)p) >> 4);
        }
      }
      if (!(characteristics & CH_JUMPING)) lift = liftAt(row(), x);

      const Lane &L = laneAt(row());

      if (!(characteristics & CH_JUMPING) && L.type == LANE_WATER)
      {
        x += L.dx;                                         // ride the log
        if (isLeafLane(L))
        {
          int16_t u = x >> 3, v = row();
          if (leafAt(L, u) && (!sinkOn || sinkV != v || sinkU != u))
          {
            sinkOn = true; sinkV = v; sinkU = u; sinkT = 0;   // this leaf starts to sink
          }
          if (!leafUnder(v, x)) { die(false); return; }       // leaf is under water: splash
        }
        else if (!logUnder(L, x)) { die(false); return; }     // splash
        if (x < (PLAY_MIN_U * 8) - 4 || x > ((PLAY_MAX_U + 1) * 8) + 4) { die(false); return; }
      }
      else if (L.type == LANE_ROAD)
      {
        if (!invuln && vehicleHit(L, x)) { die(true); return; }       // splat
      }
      else if (L.type == LANE_RUNWAY)
      {
        if (!invuln && planeHit(L.kind ? laneAt(row() - 1) : L, x)) { dieFeathers(); return; }       // feathers burst from the chicken
      }
    }

    // top-left corner of the chicken sprite on screen (17 x 24)
    void spriteTopLeft(int16_t *px, int16_t *py)
    {
      int16_t sx = (x - camX) + (y - camY) + SCREEN_CX;
      int16_t sy = (((x - camX) - (y - camY)) >> 1) + SCREEN_CY;
      *px = sx - CHICKEN_FOOT_X;
      *py = sy - CHICKEN_FOOT_Y - pgm_read_byte(&playerY[jumpCounter]) - lift - (dropT ? pgm_read_byte(&dropH[DROP_LEN - dropT]) : 0);
    }

    // splash of pixels where the chicken went into the water (drawn on top of everything)
    void drawFeathers()                                // little cubes burst from the centre, then rain down
    {
      if (deadTimer > 40) return;
      int16_t px, py;
      spriteTopLeft(&px, &py);
      int16_t cx = px + 8, cy = py + 12;
      for (byte i = 0; i < 28; i++)
      {
        byte r = splashSeed + i * 59;
        r ^= r << 3; r ^= r >> 5; r ^= r << 4; byte r1 = r;
        r ^= r << 3; r ^= r >> 5; r ^= r << 4; byte r2 = r;
        int8_t vx = (int8_t)(r1 % 19) - 9;
        int8_t vy = (int8_t)(r2 % 15) - 4;
        int16_t t = deadTimer;
        int16_t dx = (vx * t) >> 2;
        int16_t dy = ((vy * t) >> 1) - ((t * t) >> 3);
        if (dy < -14) continue;
        int16_t sx = cx + dx, sy = cy - dy;
        if (r1 & 1)
        {
          arduboy.fillRect(sx - 1, sy - 1, 3, 3, BLACK);
          arduboy.drawPixel(sx, sy, WHITE);
        }
        else
        {
          arduboy.drawPixel(sx, sy, BLACK);
          arduboy.drawPixel(sx + 1, sy, WHITE);
        }
      }
    }

    void drawSplash()
    {
      if ((characteristics & CH_DEAD) && (characteristics & CH_FEATHERS)) { drawFeathers(); return; }
      if (!(characteristics & CH_DEAD) || (characteristics & (CH_SPLAT | CH_LOST)) || deadTimer > 38) return;
      int16_t px, py;
      spriteTopLeft(&px, &py);
      int16_t cx = px + CHICKEN_FOOT_X, cy = py + CHICKEN_FOOT_Y;       // centre of where the feet were
      for (byte i = 0; i < SPLASH_DROPS; i++)
      {
        // a few random numbers per drop, from the seed of this splash (same drop = same numbers every frame)
        byte r = splashSeed + i * 59;
        r ^= r << 3; r ^= r >> 5; r ^= r << 4; byte r1 = r;
        r ^= r << 3; r ^= r >> 5; r ^= r << 4; byte r2 = r;
        r ^= r << 3; r ^= r >> 5; r ^= r << 4; byte r3 = r;
        r ^= r << 3; r ^= r >> 5; r ^= r << 4; byte r4 = r;

        int8_t  vx = (int8_t)(r1 % 17) - 8;                             // sideways speed  -8..8  (used once the column has risen)
        int8_t  vy = 5 + (r2 % 8);                                      // upwards speed    5..12
        int8_t  x0 = (int8_t)(r3 % 5) - 2;                              // starts as a narrow column around the feet
        int8_t  y0 = (int8_t)(r4 % 5) - 2;
        byte    t  = deadTimer - (r1 >> 6);                             // some drops start a few frames late
        if (deadTimer < (r1 >> 6)) continue;
        int16_t dx = (t > 5) ? (vx * (int16_t)(t - 5)) >> 3 : 0;        // first straight up, then it spreads out
        int16_t dy = ((vy * (int16_t)t) >> 2) - ((int16_t)t * t >> 4);  // gravity
        if (dy < -3) continue;                                          // fell back into the water
        int16_t sx = cx + x0 + dx, sy = cy + y0 - dy;
        if ((r2 >> 4) < 5)                                              // some big drops: white dot, black outline
        {
          arduboy.fillRect(sx - 1, sy - 1, 3, 3, BLACK);
          arduboy.drawPixel(sx, sy, WHITE);
        }
        else                                                            // small drops: black + white pixel
        {
          arduboy.drawPixel(sx, sy, BLACK);
          arduboy.drawPixel(sx + (r4 & 1 ? 1 : -1), sy - 1, WHITE);
        }
      }
    }

    // flat chicken when it has been run over
    const unsigned char* spriteBitmap()
    {
      return (characteristics & CH_SPLAT) ? chickenFlatSprite : Chicken_plus_mask;
    }

    byte spriteFrame()
    {
      if (characteristics & CH_SPLAT) return 0;
      return pgm_read_byte(&playerJumpPattern[jumpCounter]) +
             (3 * (characteristics & CH_DIR_MASK));
    }

    // draws the chicken sprite (called from the playfield, in the right order)
    void drawSprite()
    {
      if (!(characteristics & CH_VISIBLE)) return;
      if (invuln && !dropT && ((invuln >> 2) & 1)) return;                  // blinking while protected
      int16_t px, py;
      spriteTopLeft(&px, &py);
      sprites.drawPlusMask(px, py, spriteBitmap(), spriteFrame());
    }
};

chickenRunner player;

#endif
