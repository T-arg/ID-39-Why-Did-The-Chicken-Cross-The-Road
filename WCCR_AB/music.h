#ifndef MUSIC_H
#define MUSIC_H

// Music and sound effects (ATMlib).
//   bassFun   title screen: the original Bass Fun from the ATMlib examples (4 channels), loops forever
// Sound effects are one-track ATM_SFX_TRACKs for ATM.playSfx() (ATMlib has ONE effect slot, a new effect replaces the running one).

#include <ATMcmds.h>

#ifndef Song
#define Song const uint8_t PROGMEM
#endif

// ---------------------------------------------------------------------------
// Bass Fun - Italo disco groove (after the demo "Bass Fun")
// D major / mixolydian, 8 bars (512 ticks, about 18 s), tempo 28 (105 BPM): D A | C G | D A | C G, twice.
// CH0 pad (slow arpeggio chords), CH1 kick, CH2 saw bass with octave jumps, CH3 noise clap + hats + a roll.
// 212 bytes
// ---------------------------------------------------------------------------
Song bassFun[] = {          // total song in bytes = 212
  0x0F,                         // Number of tracks
  0x00, 0x00,                   // Address of track 0   //0  drums
  0x07, 0x00,                   // Address of track 1   //7  drum4
  0x0F, 0x00,                   // Address of track 2   //15  pair
  0x14, 0x00,                   // Address of track 3   //20  hat
  0x1D, 0x00,                   // Address of track 4   //29  clap
  0x28, 0x00,                   // Address of track 5   //40  roll
  0x43, 0x00,                   // Address of track 6   //67  kicks
  0x48, 0x00,                   // Address of track 7   //72  kick
  0x53, 0x00,                   // Address of track 8   //83  pad
  0x58, 0x00,                   // Address of track 9   //88  pad4
  0x70, 0x00,                   // Address of track 10   //112  bass
  0x77, 0x00,                   // Address of track 11   //119  bass4
  0x88, 0x00,                   // Address of track 12   //136  bar
  0x95, 0x00,                   // Address of track 13   //149  beat1
  0xA4, 0x00,                   // Address of track 14   //164  beat

  0x08,                         // Channel 0 entry track (pad)
  0x06,                         // Channel 1 entry track (kicks)
  0x0A,                         // Channel 2 entry track (bass)
  0x00,                         // Channel 3 entry track (drums)

  //"Track 0" drums  [7 bytes]
  ATM_SET_TEMPO(28),
  ATM_REPEAT(1, 1),   // 2x drum4
  ATM_GOTO(0),   // -> drums

  //"Track 1" drum4  [8 bytes]
  ATM_REPEAT(6, 2),   // 7x pair
  ATM_GOTO(3),   // -> hat
  ATM_GOTO(5),   // -> roll
  ATM_RETURN,

  //"Track 2" pair  [5 bytes]
  ATM_GOTO(3),   // -> hat
  ATM_GOTO(4),   // -> clap
  ATM_RETURN,

  //"Track 3" hat  [9 bytes]
  ATM_VOL(0),
  ATM_DELAY(8),
  ATM_VOL(14),
  ATM_SL_VOL((uint8_t)-4),
  ATM_DELAY(8),
  ATM_RETURN,

  //"Track 4" clap  [11 bytes]
  ATM_VOL(44),
  ATM_SL_VOL((uint8_t)-6),
  ATM_DELAY(8),
  ATM_VOL(14),
  ATM_SL_VOL((uint8_t)-4),
  ATM_DELAY(8),
  ATM_RETURN,

  //"Track 5" roll  [27 bytes]
  ATM_VOL(22),
  ATM_SL_VOL((uint8_t)-9),
  ATM_DELAY(2),
  ATM_VOL(26),
  ATM_DELAY(2),
  ATM_VOL(30),
  ATM_DELAY(2),
  ATM_VOL(34),
  ATM_DELAY(2),
  ATM_VOL(38),
  ATM_DELAY(2),
  ATM_VOL(42),
  ATM_DELAY(2),
  ATM_VOL(46),
  ATM_DELAY(2),
  ATM_VOL(50),
  ATM_DELAY(2),
  ATM_RETURN,

  //"Track 6" kicks  [5 bytes]
  ATM_REPEAT(31, 7),   // 32x kick
  ATM_GOTO(6),   // -> kicks

  //"Track 7" kick  [11 bytes]
  ATM_ARP_OFF,
  ATM_SL_FRQ((uint8_t)-127),
  ATM_VOL(40),
  ATM_NOTE_D3_,
  ATM_DELAY(4),
  ATM_VOL(0),
  ATM_DELAY(12),
  ATM_RETURN,

  //"Track 8" pad  [5 bytes]
  ATM_REPEAT(1, 9),   // 2x pad4
  ATM_GOTO(8),   // -> pad

  //"Track 9" pad4  [24 bytes]
  ATM_VOL(24),
  ATM_SL_VOL((uint8_t)-1),
  ATM_ARP(0x43, 0x23),
  ATM_NOTE_D3,
  ATM_DELAY(32),
  ATM_NOTE_A2,
  ATM_DELAY(32),
  ATM_NOTE_C3,
  ATM_DELAY(32),
  ATM_NOTE_G2,
  ATM_DELAY(32),
  ATM_NOTE_D3,
  ATM_DELAY(32),
  ATM_NOTE_A2,
  ATM_DELAY(32),
  ATM_NOTE_C3,
  ATM_DELAY(32),
  ATM_NOTE_G2,
  ATM_DELAY(32),
  ATM_RETURN,

  //"Track 10" bass  [7 bytes]
  ATM_SL_VOL((uint8_t)-2),
  ATM_REPEAT(1, 11),   // 2x bass4
  ATM_GOTO(10),   // -> bass

  //"Track 11" bass4  [17 bytes]
  ATM_SET_TRA(0),
  ATM_GOTO(12),   // -> bar
  ATM_SET_TRA((uint8_t)-2),
  ATM_GOTO(12),   // -> bar
  ATM_SET_TRA(0),
  ATM_GOTO(12),   // -> bar
  ATM_SET_TRA((uint8_t)-2),
  ATM_GOTO(12),   // -> bar
  ATM_RETURN,

  //"Track 12" bar  [13 bytes]
  ATM_GOTO(13),   // -> beat1
  ATM_GOTO(14),   // -> beat
  ATM_ADD_TRA(7),
  ATM_GOTO(14),   // -> beat
  ATM_GOTO(14),   // -> beat
  ATM_ADD_TRA((uint8_t)-7),
  ATM_RETURN,

  //"Track 13" beat1  [15 bytes]
  ATM_VOL(0),
  ATM_DELAY(4),
  ATM_VOL(56),
  ATM_NOTE_D3,
  ATM_DELAY(4),
  ATM_VOL(0),
  ATM_DELAY(4),
  ATM_VOL(56),
  ATM_NOTE_D2,
  ATM_DELAY(4),
  ATM_RETURN,

  //"Track 14" beat  [13 bytes]
  ATM_VOL(0),
  ATM_DELAY(4),
  ATM_VOL(56),
  ATM_NOTE_D2,
  ATM_DELAY(4),
  ATM_NOTE_D3,
  ATM_DELAY(4),
  ATM_VOL(0),
  ATM_DELAY(4),
  ATM_RETURN,

};

// an EMPTY song: it keeps the ATMlib playroutine running whenever there is no music, so that sound effects behave
// (all four channels play the same silent loop)
Song silentSong[] = {
  0x01,                         // Number of tracks
  0x00, 0x00,                   // Address of track 0
  0x00, 0x00, 0x00, 0x00,       // entry track of channels 0..3
  ATM_VOL(0),
  ATM_DELAY(64),
  ATM_GOTO(0)
};

// Sound effects: plain PROGMEM tracks (they end with ATM_STOP_CHAN) and a PROGMEM list, played with sfx(number)
// the jump: one note that glides and fades
const uint8_t chickJump[] PROGMEM = {
  ATM_VOL(48),
  ATM_SL_VOL((uint8_t)-6),
  ATM_GLIS(1),
  ATM_NOTE_C3,
  ATM_DELAY(12),
  ATM_STOP_CHAN
};

// touch down: ONE short low note that fades out
const uint8_t chickLand[] PROGMEM = {
  ATM_VOL(40),
  ATM_SL_VOL((uint8_t)-10),
  ATM_NOTE_A2,
  ATM_DELAY(4),
  ATM_STOP_CHAN
};

// corn: a coin (a short low note, then a higher one that rings out)
const uint8_t chickCoin[] PROGMEM = {
  ATM_VOL(40),
  ATM_NOTE_B3,
  ATM_DELAY(1),
  ATM_SL_VOL((uint8_t)-5),
  ATM_NOTE_E4,
  ATM_DELAY(8),
  ATM_STOP_CHAN
};

// fell in the water: noise that fades (channel 3 is the noise channel)
const uint8_t chickSplash[] PROGMEM = {
  ATM_VOL(48),
  ATM_SL_VOL((uint8_t)-3),
  ATM_DELAY(19),
  ATM_STOP_CHAN
};

// hit by a plane: a soft trembling note that swells
const uint8_t chickShopped[] PROGMEM = {
  ATM_VOL(20),
  ATM_SL_VOL(1),
  ATM_TREM(6, 0),
  ATM_DELAY(24),
  ATM_STOP_CHAN
};

#define SFX_JUMP 0
#define SFX_LAND 1
#define SFX_COIN 2
#define SFX_SPLASH 3
#define SFX_SHOPPED 4
const unsigned char * const PROGMEM soundFX[] =
{
  chickJump,
  chickLand,
  chickCoin,
  chickSplash,
  chickShopped,
};

// ATMlib does not look at the Arduboy sound setting, so we do it here
void sfx(byte n, byte ch = 1)
{
  if (arduboy.audio.enabled()) ATM.playSfx((const uint8_t *)pgm_read_word(&soundFX[n]), ch);
}

// called once per frame: the title tune starts with the T-arg logo and plays on the title screen
bool musicOn = false;
void musicUpdate()
{
  bool want = arduboy.audio.enabled() && (gameState == STATE_MENU_MAIN || (gameState == STATE_MENU_INTRO && introFrame >= INTRO_FRAMES));   // from the T-arg logo on
  if (want == musicOn) return;
  musicOn = want;
  if (want) ATM.play(bassFun);
  else ATM.play(silentSong);
}

#endif
