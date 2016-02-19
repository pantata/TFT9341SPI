/*
 ILI9341 2.2 TFT SPI library
 based on UTFT.cpp 
  
 Compatible with other UTFT libraries.
 Original library you can find at http://electronics.henningkarlsen.com/
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the CC BY-NC-SA 3.0 license.
 Please see the included documents for further information.
 */

#ifndef TFT9341_h
#define TFT9341_h

#include "Arduino.h"
#include "SPI.h"

//#define SPISPEED 20000000

#if defined(__AVR__)
	#define SPI_MODE_FAST
#elif defined(__arm__)     
     #define SPI_MODE_DMA
#endif


#define UTFT_VERSION 300
#define VERSION9341 100

#define LEFT 1
#define RIGHT 9999
#define CENTER 9998

#define PORTRAIT 0
#define LANDSCAPE 1


//pinout defs
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	#define CS 48
	#define RESET 33
	#define DC 47
	#define LED 46	
#elif defined(__SAMD21G18A__)
	#define CS 12
	#define RESET 11
	#define DC 10	
	#define LED 9	
#elif defined(__ATmega328__)
	#define CS 4
	#define RESET 5
	#define DC 6	
	#define LED 7	
#endif

#define TFT_WIDTH 240
#define TFT_HEIGHT 320

//*********************************
// COLORS
//*********************************
// VGA color palette
#define VGA_BLACK		0x0000
#define VGA_WHITE		0xFFFF
#define VGA_RED			0xF800
#define VGA_GREEN		0x0400
#define VGA_BLUE		0x001F
#define VGA_SILVER		0xC618
#define VGA_GRAY		0x8410
#define VGA_MAROON		0x8000
#define VGA_YELLOW		0xFFE0
#define VGA_OLIVE		0x8400
#define VGA_LIME		0x07E0
#define VGA_AQUA		0x07FF
#define VGA_TEAL		0x0410
#define VGA_NAVY		0x0010
#define VGA_FUCHSIA		0xF81F
#define VGA_PURPLE		0x8010
#define VGA_TRANSPARENT	0xFFFFFFFF

// All Color definitions
#define ILI9341_ALICEBLUE 0xF7DF 
#define ILI9341_ANTIQUEWHITE 0xFF5A 
#define ILI9341_AQUA 0x07FF 
#define ILI9341_AQUAMARINE 0x7FFA 
#define ILI9341_AZURE 0xF7FF 
#define ILI9341_BEIGE 0xF7BB 
#define ILI9341_BISQUE 0xFF38 
#define ILI9341_BLACK 0x0000 
#define ILI9341_BLANCHEDALMOND 0xFF59 
#define ILI9341_BLUE 0x001F 
#define ILI9341_BLUEVIOLET 0x895C 
#define ILI9341_BROWN 0xA145 
#define ILI9341_BURLYWOOD 0xDDD0 
#define ILI9341_CADETBLUE 0x5CF4 
#define ILI9341_CHARTREUSE 0x7FE0 
#define ILI9341_CHOCOLATE 0xD343 
#define ILI9341_CORAL 0xFBEA 
#define ILI9341_CORNFLOWERBLUE 0x64BD 
#define ILI9341_CORNSILK 0xFFDB 
#define ILI9341_CRIMSON 0xD8A7 
#define ILI9341_CYAN 0x07FF 
#define ILI9341_DARKBLUE 0x0011 
#define ILI9341_DARKCYAN 0x0451 
#define ILI9341_DARKGOLDENROD 0xBC21 
#define ILI9341_DARKGRAY 0xAD55 
#define ILI9341_DARKGREEN 0x0320 
#define ILI9341_DARKKHAKI 0xBDAD 
#define ILI9341_DARKMAGENTA 0x8811 
#define ILI9341_DARKOLIVEGREEN 0x5345 
#define ILI9341_DARKORANGE 0xFC60 
#define ILI9341_DARKORCHID 0x9999 
#define ILI9341_DARKRED 0x8800 
#define ILI9341_DARKSALMON 0xECAF 
#define ILI9341_DARKSEAGREEN 0x8DF1 
#define ILI9341_DARKSLATEBLUE 0x49F1 
#define ILI9341_DARKSLATEGRAY 0x2A69 
#define ILI9341_DARKTURQUOISE 0x067A 
#define ILI9341_DARKVIOLET 0x901A 
#define ILI9341_DEEPPINK 0xF8B2 
#define ILI9341_DEEPSKYBLUE 0x05FF 
#define ILI9341_DIMGRAY 0x6B4D 
#define ILI9341_DODGERBLUE 0x1C9F 
#define ILI9341_FIREBRICK 0xB104 
#define ILI9341_FLORALWHITE 0xFFDE 
#define ILI9341_FORESTGREEN 0x2444 
#define ILI9341_FUCHSIA 0xF81F 
#define ILI9341_GAINSBORO 0xDEFB 
#define ILI9341_GHOSTWHITE 0xFFDF 
#define ILI9341_GOLD 0xFEA0 
#define ILI9341_GOLDENROD 0xDD24 
#define ILI9341_GRAY 0x8410 
#define ILI9341_GREEN 0x0400 
#define ILI9341_GREENYELLOW 0xAFE5 
#define ILI9341_HONEYDEW 0xF7FE 
#define ILI9341_HOTPINK 0xFB56 
#define ILI9341_INDIANRED 0xCAEB 
#define ILI9341_INDIGO 0x4810 
#define ILI9341_IVORY 0xFFFE 
#define ILI9341_KHAKI 0xF731 
#define ILI9341_LAVENDER 0xE73F 
#define ILI9341_LAVENDERBLUSH 0xFF9E 
#define ILI9341_LAWNGREEN 0x7FE0 
#define ILI9341_LEMONCHIFFON 0xFFD9 
#define ILI9341_LIGHTBLUE 0xAEDC 
#define ILI9341_LIGHTCORAL 0xF410 
#define ILI9341_LIGHTCYAN 0xE7FF 
#define ILI9341_LIGHTGOLDENRODYELLOW 0xFFDA 
#define ILI9341_LIGHTGREEN 0x9772 
#define ILI9341_LIGHTGREY 0xD69A 
#define ILI9341_LIGHTPINK 0xFDB8 
#define ILI9341_LIGHTSALMON 0xFD0F 
#define ILI9341_LIGHTSEAGREEN 0x2595 
#define ILI9341_LIGHTSKYBLUE 0x867F 
#define ILI9341_LIGHTSLATEGRAY 0x7453 
#define ILI9341_LIGHTSTEELBLUE 0xB63B 
#define ILI9341_LIGHTYELLOW 0xFFFC 
#define ILI9341_LIME 0x07E0 
#define ILI9341_LIMEGREEN 0x3666 
#define ILI9341_LINEN 0xFF9C 
#define ILI9341_MAGENTA 0xF81F 
#define ILI9341_MAROON 0x8000 
#define ILI9341_MEDIUMAQUAMARINE 0x6675 
#define ILI9341_MEDIUMBLUE 0x0019 
#define ILI9341_MEDIUMORCHID 0xBABA 
#define ILI9341_MEDIUMPURPLE 0x939B 
#define ILI9341_MEDIUMSEAGREEN 0x3D8E 
#define ILI9341_MEDIUMSLATEBLUE 0x7B5D 
#define ILI9341_MEDIUMSPRINGGREEN 0x07D3 
#define ILI9341_MEDIUMTURQUOISE 0x4E99 
#define ILI9341_MEDIUMVIOLETRED 0xC0B0 
#define ILI9341_MIDNIGHTBLUE 0x18CE 
#define ILI9341_MINTCREAM 0xF7FF 
#define ILI9341_MISTYROSE 0xFF3C 
#define ILI9341_MOCCASIN 0xFF36 
#define ILI9341_NAVAJOWHITE 0xFEF5 
#define ILI9341_NAVY 0x0010 
#define ILI9341_OLDLACE 0xFFBC 
#define ILI9341_OLIVE 0x8400 
#define ILI9341_OLIVEDRAB 0x6C64 
#define ILI9341_ORANGE 0xFD20 
#define ILI9341_ORANGERED 0xFA20 
#define ILI9341_ORCHID 0xDB9A 
#define ILI9341_PALEGOLDENROD 0xEF55 
#define ILI9341_PALEGREEN 0x9FD3 
#define ILI9341_PALETURQUOISE 0xAF7D 
#define ILI9341_PALEVIOLETRED 0xDB92 
#define ILI9341_PAPAYAWHIP 0xFF7A 
#define ILI9341_PEACHPUFF 0xFED7 
#define ILI9341_PERU 0xCC27 
#define ILI9341_PINK 0xFE19 
#define ILI9341_PLUM 0xDD1B 
#define ILI9341_POWDERBLUE 0xB71C 
#define ILI9341_PURPLE 0x8010 
#define ILI9341_RED 0xF800 
#define ILI9341_ROSYBROWN 0xBC71 
#define ILI9341_ROYALBLUE 0x435C 
#define ILI9341_SADDLEBROWN 0x8A22 
#define ILI9341_SALMON 0xFC0E 
#define ILI9341_SANDYBROWN 0xF52C 
#define ILI9341_SEAGREEN 0x2C4A 
#define ILI9341_SEASHELL 0xFFBD 
#define ILI9341_SIENNA 0xA285 
#define ILI9341_SILVER 0xC618 
#define ILI9341_SKYBLUE 0x867D 
#define ILI9341_SLATEBLUE 0x6AD9 
#define ILI9341_SLATEGRAY 0x7412 
#define ILI9341_SNOW 0xFFDF 
#define ILI9341_SPRINGGREEN 0x07EF 
#define ILI9341_STEELBLUE 0x4416 
#define ILI9341_TAN 0xD5B1 
#define ILI9341_TEAL 0x0410 
#define ILI9341_THISTLE 0xDDFB 
#define ILI9341_TOMATO 0xFB08 
#define ILI9341_TURQUOISE 0x471A 
#define ILI9341_VIOLET 0xEC1D 
#define ILI9341_WHEAT 0xF6F6 
#define ILI9341_WHITE 0xFFFF 
#define ILI9341_WHITESMOKE 0xF7BE 
#define ILI9341_YELLOW 0xFFE0 
#define ILI9341_YELLOWGREEN 0x9E66

//commands
#define ILI9341_VSCRDEF 0x33
#define ILI9341_VSCRSADD 0x37

#define NOP     0x00
#define SWRESET 0x01
#define RDDID   0x04
#define RDDST   0x09

#define SLPIN   0x10
#define SLPOUT  0x11
#define PTLON   0x12
#define NORON   0x13

#define RDMODE  0x0A
#define RDMADCTL  0x0B
#define RDPIXFMT  0x0C
#define RDIMGFMT  0x0A
#define RDSELFDIAG  0x0F

#define INVOFF  0x20
#define INVON   0x21
#define GAMMASET 0x26
#define DISPOFF 0x28
#define DISPON  0x29

#define CASET   0x2A
#define PASET   0x2B
#define RAMWR   0x2C
#define RAMRD   0x2E

#define PTLAR   0x30
#define MADCTL  0x36
#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04

#define PIXFMT  0x3A

#define FRMCTR1 0xB1
#define FRMCTR2 0xB2
#define FRMCTR3 0xB3
#define INVCTR  0xB4
#define DFUNCTR 0xB6

#define PWCTR1  0xC0
#define PWCTR2  0xC1
#define PWCTR3  0xC2
#define PWCTR4  0xC3
#define PWCTR5  0xC4
#define VMCTR1  0xC5
#define VMCTR2  0xC7

#define RDID1   0xDA
#define RDID2   0xDB
#define RDID3   0xDC
#define RDID4   0xDD

#define GMCTRP1 0xE0
#define GMCTRN1 0xE1

#define swap(type, i, j) {type t = i; i = j; j = t;}
#define fontbyte(x) pgm_read_byte(&cfont.font[x])

struct _current_font
{
	uint8_t* font;
	uint8_t x_size;
	uint8_t y_size;
	uint8_t offset;
	uint8_t numchars;
};

class propFont
{
    public:
        byte charCode;
        int adjYOffset;
        int width;
        int height;
        int xOffset;
        int xDelta;
        byte* dataPtr;
};

class TFT9341
{
	public:	
		TFT9341();
		void InitLCD(byte orientation=LANDSCAPE);
		void setRotation(uint8_t m);
		int  getDisplayXSize();
		int	 getDisplayYSize();
				
		void clrScr();
		void drawRect(int x1, int y1, int x2, int y2);
		
		void drawPixel(int x, int y);
		void drawLine(int x1, int y1, int x2, int y2);
		
		void setColor(uint8_t r, uint8_t g, uint8_t b);
		void setColor(word color);		
		uint16_t getColor();
		void setBackColor(uint8_t r, uint8_t g, uint8_t b);
		void setBackColor(uint32_t color);
		uint16_t getBackColor();
		
		void fillScr(uint8_t r, uint8_t g, uint8_t b);
		void fillScr(uint16_t color);	
		void fillRect(int x1, int y1, int x2, int y2);		
		
        void drawRoundRect(int x1, int y1, int x2, int y2);
        void fillRoundRect(int x1, int y1, int x2, int y2);		
        
		void drawCircle(int x, int y, int radius);
		void fillCircle(int x, int y, int radius);
		
		void setFont(uint8_t* font);
		uint8_t* getFont();
		uint8_t getFontXsize();
		uint8_t getFontYsize();
		
		void lcdOff();
		void lcdOn();	
		void print(String st, int x, int y, int deg=0);		
		void print(char *st, int x, int y, int deg=0);

		void rotateChar(byte c, int x, int y, int pos, int deg);
		void printNumI(long num, int x, int y, int length=0, char filler=' ');        
		
	    //prop font
	    int getStringWidth(char* str);
      	int getFontHeight();
      	byte getOrientation();
      	
      	//
    	//void readRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t *pcolors);
   		//void writeRect(int16_t x, int16_t y, int16_t w, int16_t h, const uint8_t *pcolors);
      	

/*						
		void	drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
		void	fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
		void	drawArc(int x, int y, int r, int startAngle, int endAngle, int thickness = 1);
		void	drawPie(int x, int y, int r, int startAngle, int endAngle);
        void    fillPie(int x, int y, int r, int startAngle, int endAngle);		
		void printNumF(double num, byte dec, int x, int y, char divider='.', int length=0, char filler=' ');				
		void drawBitmap(int x, int y, int sx, int sy, bitmapdatatype data, int scale=1);
		void drawBitmap(int x, int y, int sx, int sy, bitmapdatatype data, int deg, int rox, int roy);
		        
	   	//void setContrast(char c);

        //hardware vertical scoll
      	void setupScrollArea(uint16_t TFA, uint16_t BFA);  
   	  	void scrollAddress(uint16_t VSP);
		void _convert_float(char *buf, double num, int width, byte prec);   	   		
*/	
	protected:
		int printProportionalChar(byte c, int x, int y);
		int rotatePropChar(byte c, int x, int y, int offset, int deg);
		
   private:
		bool getCharPtr(byte c, propFont& font);
		byte fch, fcl, bch, bcl;
		byte orient;
		long disp_x_size, disp_y_size;
		_current_font	cfont;
		boolean _transparent;
		uint8_t rotation;
		uint16_t scanline[320];
		uint16_t _fgc, _bgc;

		void setXY(word x1, word y1, word x2, word y2);
		void drawHLine(int x, int y, int l);
		void drawHLine_noCS(int x, int y, int l);
		void drawVLine(int x, int y, int l);		
		void drawVLine_noCS(int x, int y, int l);
		void drawPixel_noCS(int x, int y);
		void setPixel(uint16_t color);
		void printChar(byte c, int x, int y);
      
		static uint16_t color565(uint8_t r, uint8_t g, uint8_t b) {	
			return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
		}        
  
};

extern TFT9341 Tft;

#endif