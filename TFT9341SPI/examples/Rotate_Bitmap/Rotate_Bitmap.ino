// UTFT_Rotate_Bitmap (C)2012 Henning Karlsen
// web: http://www.henningkarlsen.com/electronics
//
// This program is a demo of the drawBitmap()-function.
//
// This program requires the UTFT library.
//

#include <TFT9341.h>
#include <SPI.h>

#if defined(__AVR__)
	#include <avr/pgmspace.h>
#endif


TFT9341 myGLCD;

extern unsigned int tux[0x400];

void setup()
{
  myGLCD.InitLCD();
  myGLCD.setRotation(2);
  myGLCD.fillScr(255, 255, 255);
  myGLCD.setColor(0, 0, 0);
}

void loop()
{
    for (int i=0; i<360; i+=5)
    {
      myGLCD.drawBitmap (10, 10, 32, 32, tux, i, 16, 16);
    }
}


