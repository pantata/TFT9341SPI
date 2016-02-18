// UTFT_ViewFont (C)2012 Henning Karlsen
// web: http://www.henningkarlsen.com/electronics
//
// This program is a demo of the included fonts.
//
// This demo was made for modules with a screen resolution 
// of 320x240 pixels.
//
// This program requires the UTFT library.
//

#include <TFT9341.h>
#include <SPI.h>

// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];


TFT9341 myGLCD;

void setup()
{
  myGLCD.InitLCD();
  myGLCD.setRotation(0);
  myGLCD.clrScr();
  myGLCD.setColor(0, 255, 0);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setFont(BigFont);
  myGLCD.print(" !\"#$%&'()*+,-./", CENTER, 0);
  
}

void loop()
{
}


