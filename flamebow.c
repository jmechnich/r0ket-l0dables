#include <sysinit.h>

#include "basic/basic.h"
#include "basic/idle.h"
#include "flame/flame.h"

#include "usetable.h"

// fading states
#define REDtoYELLOW 	0
#define YELLOWtoGREEN 	1
#define GREENtoCYAN 	2
#define CYANtoBLUE 	3
#define BLUEtoVIOLET 	4
#define VIOLETtoRED 	5

// min/max values
#define V_MAX 255
#define V_MIN 0

// global variables
int16_t red;
int16_t green;
int16_t blue;
uint8_t state;

// need forward declarations here so ram() gets executed as main function
void init();
void rainbowfade(int n);
void printValue( const char* name, uint8_t val);
void clamp(int16_t* col);

// main loop
void ram(void)
{
  init();

  uint8_t brightness = 15;
  uint8_t stepsize = 1;
  uint32_t btndelay = 100;

  uint8_t flame = flameDetect();

  lcdClear();
  lcdPrintln( "!! FLAMEBOW !!");
  lcdRefresh();
  delayms_queue( 10);

  if( flame == FLAME_TYPE_NONE)
  {
    lcdPrintln( "No flame found!");
    lcdRefresh();
    delayms_queue( 1000);
    return;
  }
  
  flameClaim( flame);
  flameSetBrightness( flame, brightness);
  flameSetColor( flame, red, green, blue);
  
  uint8_t button = 0;
  while( 1)
  {
    switch( button)
    {
    case( BTN_ENTER):
      flameFree( flame);
      return;
      break;
    case( BTN_RIGHT):
      if( stepsize != 128)
      {
        stepsize *= 2;
        init();
        printValue( "Stepsize: ", stepsize);
        delayms( btndelay);
      }
      break;
    case( BTN_LEFT):
      if( stepsize != 1)
      {
        stepsize /= 2;
        init();
        printValue( "Stepsize: ", stepsize);
        delayms( btndelay);
      }
      break;
    case( BTN_UP):
      if( brightness != 255)
      {
        ++brightness; 
        printValue( "Brightness: ", brightness);
        flameSetBrightness( flame, brightness);
      }
      break;
    case( BTN_DOWN):
      if( brightness != 0)
      {
        --brightness;
        printValue( "Brightness: ", brightness);
        flameSetBrightness( flame, brightness);
      }
      break;
    default:
      break;
    }
    
    rainbowfade(stepsize);
    flameSetColor( flame, red, green, blue);
    delayms_queue(10);
    button = getInputWaitTimeout( 10);
    if( button == BTN_NONE)
        delayms(10);
  }
  flameFree( flame);
}

void init()
{
  red = 255;
  green = 0;
  blue = 0;
  state = 0;
}

// successively fade r -> y -> g -> c -> b -> m -> r ...
void rainbowfade(int n)
{
  if (state==REDtoYELLOW) green+=n;
  if (state==YELLOWtoGREEN) red-=n;
  if (state==GREENtoCYAN) blue+=n;
  if (state==CYANtoBLUE) green-=n;
  if (state==BLUEtoVIOLET) red+=n;
  if (state==VIOLETtoRED) blue-=n;
  clamp( &red); clamp( &green); clamp( &blue);
  
  if (red==V_MAX || green==V_MAX || blue==V_MAX ||
      red==0 || green==0 || blue==0)
  {
    // fade to next color
    state++;
    state%=6;
  }
}

// print a name/value pair to the lcd
void printValue( const char* name, uint8_t val)
{
  lcdPrint( name );
  lcdPrint( IntToStr(val,3,0) );
  lcdPrintln( "");
  lcdRefresh();
}

// clamp int16_t to uint8_t range
void clamp( int16_t* col)
{
  if( *col > 255) *col = 255;
  else if( *col < 0) *col = 0;
}
