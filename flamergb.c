#include <sysinit.h>

#include "basic/basic.h"
#include "basic/idle.h"
#include "flame/flame.h"

#include "usetable.h"

// min/max values
#define V_MAX 255
#define V_MIN 0

// main colors
static const uint8_t RED[3]     = { V_MAX, V_MIN, V_MIN };
static const uint8_t YELLOW[3]  = { V_MAX, V_MAX, V_MIN };
static const uint8_t GREEN[3]   = { V_MIN, V_MAX, V_MIN };
static const uint8_t CYAN[3]    = { V_MIN, V_MAX, V_MAX };
static const uint8_t BLUE[3]    = { V_MIN, V_MIN, V_MAX };
static const uint8_t MAGENTA[3] = { V_MAX, V_MIN, V_MAX };

// need forward declarations here so ram() gets executed as main function
void flameFadeIn( uint8_t flame);
void flameFadeOut( uint8_t flame);
void flameSetColorArray( uint8_t flame, const uint8_t* rgb);

// main loop
void ram(void)
{
  lcdClear();
  lcdRefresh();
  
  uint8_t flame = flameDetect();

  if( flame == FLAME_TYPE_NONE)
  {
    lcdPrintln( "No flame found!");
    lcdRefresh();
    delayms_queue( 1000);
    return;
  }
  
  flameClaim( flame);
  flameSetBrightness( flame, 0);

  lcdPrintln( "Red"); lcdRefresh();
  flameSetColorArray( flame, RED);
  flameFadeIn(flame); flameFadeOut(flame);
  
  lcdPrintln( "Yellow"); lcdRefresh();
  flameSetColorArray( flame, YELLOW);
  flameFadeIn(flame); flameFadeOut(flame);
  
  lcdPrintln( "Green"); lcdRefresh();
  flameSetColorArray( flame, GREEN);
  flameFadeIn(flame); flameFadeOut(flame);
  
  lcdPrintln( "Cyan"); lcdRefresh();
  flameSetColorArray( flame, CYAN);
  flameFadeIn(flame); flameFadeOut(flame);
  
  lcdPrintln( "Blue"); lcdRefresh();
  flameSetColorArray( flame, BLUE);
  flameFadeIn(flame); flameFadeOut(flame);
  
  lcdPrintln( "Magenta"); lcdRefresh();
  flameSetColorArray( flame, MAGENTA);
  flameFadeIn(flame); flameFadeOut(flame);
  
  lcdPrintln( "Off"); lcdRefresh();
  delayms_queue( 1000);

  flameFree( flame);
};

void flameFadeIn( uint8_t flame)
{
  for( int16_t i=0; i<256; i+=10)
  {
    flameSetBrightness( flame, i);
    delayms_queue(50);
  }
  flameSetBrightness( flame, 255);
  delayms_queue(50);
}

void flameFadeOut(uint8_t flame)
{
  for( int16_t i=255; i>=0; i-=10)
  {
    flameSetBrightness( flame, i);
    delayms_queue(50);
  }
  flameSetBrightness( flame, 0);
  delayms_queue(50);
}

void flameSetColorArray( uint8_t flame, const uint8_t* rgb)
{
  flameSetColor( flame, rgb[0], rgb[1], rgb[2]);
  delayms_queue( 50);
}
