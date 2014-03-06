#include <sysinit.h>

#include "basic/basic.h"
#include "basic/idle.h"
#include "flame/flame.h"

#include "usetable.h"

uint8_t sintable[128] =
{
    0, 3, 6, 9, 13, 16, 19, 22, 25, 28, 31, 34, 37, 41, 44, 47, 50, 53, 56, 59,
    62, 65, 68, 71, 74, 77, 80, 83, 86, 89, 92, 95, 98, 100, 103, 106, 109,
    112, 115, 117, 120, 123, 126, 128, 131, 134, 136, 139, 142, 144, 147, 149,
    152, 154, 157, 159, 162, 164, 167, 169, 171, 174, 176, 178, 180, 183, 185,
    187, 189, 191, 193, 195, 197, 199, 201, 203, 205, 207, 208, 210, 212, 214,
    215, 217, 219, 220, 222, 223, 225, 226, 228, 229, 231, 232, 233, 234, 236,
    237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 247, 248, 249, 249,
    250, 251, 251, 252, 252, 253, 253, 253, 254, 254, 254, 255, 255, 255, 255,
    255
};

//Fading States
#define REDtoYELLOW 	0
#define YELLOWtoGREEN 	1
#define GREENtoCYAN 	2
#define CYANtoBLUE 	3
#define BLUEtoVIOLET 	4
#define VIOLETtoRED 	5

//Maximim value for led brightness
#define V_MAX 255
#define V_MIN 0

int16_t red;
int16_t green;
int16_t blue;
uint8_t state;

uint8_t brightness;

void init();
void runTest( uint8_t flame);
void rainbowfade(int n);
void printValue( const char* name, uint8_t val);
void flameFadeIn( uint8_t flame);
void flameFadeOut(uint8_t flame);
void flameFadeInSin( uint8_t flame);
void flameFadeOutSin(uint8_t flame);
void clamp(int16_t* col);
void flameSetColorArray( uint8_t flame, const uint8_t* rgb);

static const uint8_t RED[3] = { 255, 0, 0};
static const uint8_t YELLOW[3] = { 255, 255, 0};
static const uint8_t GREEN[3] = { 0, 255, 0};
static const uint8_t CYAN[3] = { 0, 255, 255};
static const uint8_t BLUE[3] = { 0, 0, 255};
static const uint8_t MAGENTA[3] = { 255, 0, 255};
static const uint8_t BLACK[3] = { 0, 0, 0};
static const uint8_t WHITE[3] = { 0, 0, 0};

void ram(void)
{
  lcdClear();
  lcdRefresh();
  
  uint8_t flame = flameDetect();
  uint8_t time = 10;
  uint8_t stepsize = 1;
  uint32_t delay = 100;
  
  init();
  
  if( flame == FLAME_TYPE_RGB)
  {
    flameClaim( flame);
    flameSetBrightness( flame, 0);

    // test flame
    runTest( flame);

    lcdPrintln( "!!! RAINBOW !!!");
    lcdRefresh();
    delayms_queue( 10);
    
    flameSetColor( flame, red, green, blue);
    flameFadeIn( flame);
    
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
          delayms( delay);
        }
        break;
      case( BTN_LEFT):
        if( stepsize != 1)
        {
          stepsize /= 2;
          init();
          printValue( "Stepsize: ", stepsize);
          delayms( delay);
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
      button = getInputWaitTimeout( time);
      if( button == BTN_NONE)
          delayms(10);
    }
    flameFree( flame);
  }
};

void init()
{
  red = 255;
  green = 0;
  blue = 0;
  state = 0;
  brightness = 255;
}

void runTest( uint8_t flame)
{
  lcdPrintln( "Red"); lcdRefresh();
  flameSetColorArray( flame, RED);
  flameFadeInSin(flame);
  flameFadeOutSin(flame);
  lcdPrintln( "Yellow"); lcdRefresh();
  flameSetColorArray( flame, YELLOW);
  flameFadeInSin(flame);
  flameFadeOutSin(flame);
  lcdPrintln( "Green"); lcdRefresh();
  flameSetColorArray( flame, GREEN);
  flameFadeInSin(flame);
  flameFadeOutSin(flame);
  lcdPrintln( "Cyan"); lcdRefresh();
  flameSetColorArray( flame, CYAN);
  flameFadeInSin(flame);
  flameFadeOutSin(flame);
  lcdPrintln( "Blue"); lcdRefresh();
  flameSetColorArray( flame, BLUE);
  flameFadeInSin(flame);
  flameFadeOutSin(flame);
  lcdPrintln( "Magenta"); lcdRefresh();
  flameSetColorArray( flame, MAGENTA);
  flameFadeInSin(flame);
  flameFadeOutSin(flame);
  lcdPrintln( "Off"); lcdRefresh();
  delayms_queue( 1000);
  lcdClear(); lcdRefresh();
  delayms_queue( 50);
}

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
    //Finished fading a color; move on to the next
    state++;
    state%=6;
  }
}

void printValue( const char* name, uint8_t val)
{
  lcdPrint( name );
  lcdPrint( IntToStr(val,3,0) );
  lcdPrintln( "");
  lcdRefresh();
}

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

void flameFadeInSin( uint8_t flame)
{
  uint8_t* pos = &(*sintable);
  uint8_t counter = 0;
  while(1)
  {
    flameSetBrightness( flame, *pos);
    delayms_queue(50);
    ++pos;
    if( counter == 127) break;
    else ++counter;
  }
  flameSetBrightness( flame, 255);
  delayms_queue(50);
}

void flameFadeOutSin(uint8_t flame)
{
  uint8_t* pos = &*(sintable+127);
  uint8_t counter = 0;
  while(1)
  {
    flameSetBrightness( flame, *pos);
    delayms_queue(50);
    --pos;
    if( counter == 127) break;
    else ++counter;
  }
  flameSetBrightness( flame, 0);
  delayms_queue(50);
}

void clamp(int16_t* col)
{
  if( *col > 255) *col = 255;
  else if( *col < 0) *col = 0;
}

void flameSetColorArray( uint8_t flame, const uint8_t* rgb)
{
  flameSetColor( flame, rgb[0], rgb[1], rgb[2]);
  delayms_queue( 50);
}

