// UTFT_Demo_320x240 (C)2012 Henning Karlsen
// web: http://www.henningkarlsen.com/electronics
//
// This program is a demo of how to use most of the functions
// of the library with a supported display modules.
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


//TFT9341m0 Tft;   


void setup()
{
  randomSeed(analogRead(0));
  
// Setup the LCD
  //delay(1000);
  Tft.InitLCD(3);
  Tft.setFont(SmallFont);
}

void loop()
{
  int buf[318];
  int x, x2;
  int y, y2;
  int r;

// Clear the screen and draw the frame
  Tft.clrScr();

  Tft.setColor(255, 0, 0);
  Tft.fillRect(0, 0, 319, 13);
  Tft.setColor(64, 64, 64);
  Tft.fillRect(0, 226, 319, 239);
  Tft.setColor(255, 255, 255);
  Tft.setBackColor(255, 0, 0);
  Tft.print("* Universal Color TFT Display Library *", CENTER, 1);
  Tft.setBackColor(64, 64, 64);
  Tft.setColor(255,255,0);
  Tft.print("<http://electronics.henningkarlsen.com>", CENTER, 227);

  Tft.setColor(0, 0, 255);
  Tft.drawRect(0, 14, 319, 225);

// Draw crosshairs
  Tft.setColor(0, 0, 255);
  Tft.setBackColor(0, 0, 0);
  Tft.drawLine(159, 15, 159, 224);
  Tft.drawLine(1, 119, 318, 119);
  for (int i=9; i<310; i+=10)
    Tft.drawLine(i, 117, i, 121);
  for (int i=19; i<220; i+=10)
    Tft.drawLine(157, i, 161, i);

// Draw sin-, cos- and tan-lines  
  Tft.setColor(0,255,255);
  Tft.print("Sin", 5, 15);
  for (int i=1; i<318; i++)
  {
    Tft.drawPixel(i,119+(sin(((i*1.13)*3.14)/180)*95));
  }
  
  Tft.setColor(255,0,0);
  Tft.print("Cos", 5, 27);
  for (int i=1; i<318; i++)
  {
    Tft.drawPixel(i,119+(cos(((i*1.13)*3.14)/180)*95));
  }

  Tft.setColor(255,255,0);
  Tft.print("Tan", 5, 39);
  for (int i=1; i<318; i++)
  {
    Tft.drawPixel(i,119+(tan(((i*1.13)*3.14)/180)));
  }

  //delay(2000);

  Tft.setColor(0,0,0);
  Tft.fillRect(1,15,318,224);
  Tft.setColor(0, 0, 255);
  Tft.setBackColor(0, 0, 0);
  Tft.drawLine(159, 15, 159, 224);
  Tft.drawLine(1, 119, 318, 119);

// Draw a moving sinewave
  x=1;
  for (int i=1; i<(318*20); i++) 
  {
    x++;
    if (x==319)
      x=1;
    if (i>319)
    {
      if ((x==159)||(buf[x-1]==119))
        Tft.setColor(0,0,255);
      else
        Tft.setColor(0,0,0);
      Tft.drawPixel(x,buf[x-1]);
    }
    Tft.setColor(0,255,255);
    y=119+(sin(((i*1.1)*3.14)/180)*(90-(i / 100)));
    Tft.drawPixel(x,y);
    buf[x-1]=y;
  }

  //delay(2000);
  
  Tft.setColor(0,0,0);
  Tft.fillRect(1,15,318,224);

// Draw some filled rectangles
  for (int i=1; i<6; i++)
  {
    switch (i)
    {
      case 1:
        Tft.setColor(255,0,255);
        break;
      case 2:
        Tft.setColor(255,0,0);
        break;
      case 3:
        Tft.setColor(0,255,0);
        break;
      case 4:
        Tft.setColor(0,0,255);
        break;
      case 5:
        Tft.setColor(255,255,0);
        break;
    }
    Tft.fillRect(70+(i*20), 30+(i*20), 130+(i*20), 90+(i*20));
  }

  //delay(2000);
  
  Tft.setColor(0,0,0);
  Tft.fillRect(1,15,318,224);

// Draw some filled, rounded rectangles
  for (int i=1; i<6; i++)
  {
    switch (i)
    {
      case 1:
        Tft.setColor(255,0,255);
        break;
      case 2:
        Tft.setColor(255,0,0);
        break;
      case 3:
        Tft.setColor(0,255,0);
        break;
      case 4:
        Tft.setColor(0,0,255);
        break;
      case 5:
        Tft.setColor(255,255,0);
        break;
    }
    Tft.fillRoundRect(190-(i*20), 30+(i*20), 250-(i*20), 90+(i*20));
  }
  
  //delay(2000);
  
  Tft.setColor(0,0,0);
  Tft.fillRect(1,15,318,224);

// Draw some filled circles
  for (int i=1; i<6; i++)
  {
    switch (i)
    {
      case 1:
        Tft.setColor(255,0,255);
        break;
      case 2:
        Tft.setColor(255,0,0);
        break;
      case 3:
        Tft.setColor(0,255,0);
        break;
      case 4:
        Tft.setColor(0,0,255);
        break;
      case 5:
        Tft.setColor(255,255,0);
        break;
    }
    Tft.fillCircle(100+(i*20),60+(i*20), 30);
  }
  
  //delay(2000);
  
  Tft.setColor(0,0,0);
  Tft.fillRect(1,15,318,224);

// Draw some lines in a pattern
  Tft.setColor (255,0,0);
  for (int i=15; i<224; i+=5)
  {
    Tft.drawLine(1, i, (i*1.44)-10, 224);
  }
  Tft.setColor (255,0,0);
  for (int i=224; i>15; i-=5)
  {
    Tft.drawLine(318, i, (i*1.44)-11, 15);
  }
  Tft.setColor (0,255,255);
  for (int i=224; i>15; i-=5)
  {
    Tft.drawLine(1, i, 331-(i*1.44), 15);
  }
  Tft.setColor (0,255,255);
  for (int i=15; i<224; i+=5)
  {
    Tft.drawLine(318, i, 330-(i*1.44), 224);
  }
  
  //delay(2000);
  
  Tft.setColor(0,0,0);
  Tft.fillRect(1,15,318,224);

// Draw some random circles
  for (int i=0; i<100; i++)
  {
    Tft.setColor(random(255), random(255), random(255));
    x=32+random(256);
    y=45+random(146);
    r=random(30);
    Tft.drawCircle(x, y, r);
  }

  //delay(2000);
  
  Tft.setColor(0,0,0);
  Tft.fillRect(1,15,318,224);

// Draw some random rectangles
  for (int i=0; i<100; i++)
  {
    Tft.setColor(random(255), random(255), random(255));
    x=2+random(316);
    y=16+random(207);
    x2=2+random(316);
    y2=16+random(207);
    Tft.drawRect(x, y, x2, y2);
  }

  //delay(2000);
  
  Tft.setColor(0,0,0);
  Tft.fillRect(1,15,318,224);

// Draw some random rounded rectangles
  for (int i=0; i<100; i++)
  {
    Tft.setColor(random(255), random(255), random(255));
    x=2+random(316);
    y=16+random(207);
    x2=2+random(316);
    y2=16+random(207);
    Tft.drawRoundRect(x, y, x2, y2);
  }

  //delay(2000);
  
  Tft.setColor(0,0,0);
  Tft.fillRect(1,15,318,224);

  for (int i=0; i<100; i++)
  {
    Tft.setColor(random(255), random(255), random(255));
    x=2+random(316);
    y=16+random(209);
    x2=2+random(316);
    y2=16+random(209);
    Tft.drawLine(x, y, x2, y2);
  }

  //delay(2000);
  
  Tft.setColor(0,0,0);
  Tft.fillRect(1,15,318,224);

  for (int i=0; i<10000; i++)
  {
    Tft.setColor(random(255), random(255), random(255));
    Tft.drawPixel(2+random(316), 16+random(209));
  }

  //delay(2000);

  Tft.fillScr(0, 0, 255);
  
  Tft.setColor(255, 0, 0);
  Tft.fillRoundRect(80, 70, 239, 169);
  
  Tft.setColor(255, 255, 255);
  Tft.setBackColor(255, 0, 0);
  Tft.print("That's it!", CENTER, 93);
  Tft.print("Restarting in a", CENTER, 119);
  Tft.print("few seconds...", CENTER, 132);
  
  Tft.setColor(0, 255, 0);
  Tft.setBackColor(0, 0, 255);
  Tft.print("Runtime: (msecs)", CENTER, 210);
  Tft.printNumI(millis(), CENTER, 225);
  
  delay (10000);
}


