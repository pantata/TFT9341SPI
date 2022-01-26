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


//pinout defs
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    #define CS 48
    #define RESET 33
    #define DC 47
    #define LED 46
#elif defined(__arm__)
    #define CS 16
    #define RESET 14
    #define DC 17
    #define LED 15
#else  //elif defined(__ATmega328__)

    #define RESET  10
    #define CS     8
    #define DC     9
    #define LED    7
/*
    #define CS 4
    #define RESET 5
    #define DC 6
    #define LED 7
*/
#endif

/*
#if defined(__AVR__)
	#define SPI_MODE_FAST
#elif defined(__arm__)     
    #define SPI_MODE_DMA
#endif
*/

#define UTFT_VERSION 300
#define VERSION9341 100

#define LEFT 1
#define RIGHT 9999
#define CENTER 9998

#define PORTRAIT 0
#define LANDSCAPE 1
#define PORTRAIT1 2
#define LANDSCAPE1 3

#define TFT_WIDTH 240
#define TFT_HEIGHT 320

//*********************************
// COLORS
//*********************************
#define COLOR_ALICEBLUE 0xF7DF 
#define COLOR_ANTIQUEWHITE 0xFF5A 
#define COLOR_AQUA 0x07FF 
#define COLOR_AQUAMARINE 0x7FFA 
#define COLOR_AZURE 0xF7FF 
#define COLOR_BEIGE 0xF7BB 
#define COLOR_BISQUE 0xFF38 
#define COLOR_BLACK 0x0000 
#define COLOR_BLANCHEDALMOND 0xFF59 
#define COLOR_BLUE 0x001F 
#define COLOR_BLUEVIOLET 0x895C 
#define COLOR_BROWN 0xA145 
#define COLOR_BURLYWOOD 0xDDD0 
#define COLOR_CADETBLUE 0x5CF4 
#define COLOR_CHARTREUSE 0x7FE0 
#define COLOR_CHOCOLATE 0xD343 
#define COLOR_CORAL 0xFBEA 
#define COLOR_CORNFLOWERBLUE 0x64BD 
#define COLOR_CORNSILK 0xFFDB 
#define COLOR_CRIMSON 0xD8A7 
#define COLOR_CYAN 0x07FF 
#define COLOR_DARKBLUE 0x0011 
#define COLOR_DARKCYAN 0x0451 
#define COLOR_DARKGOLDENROD 0xBC21 
#define COLOR_DARKGRAY 0xAD55 
#define COLOR_DARKGREEN 0x0320 
#define COLOR_DARKKHAKI 0xBDAD 
#define COLOR_DARKMAGENTA 0x8811 
#define COLOR_DARKOLIVEGREEN 0x5345 
#define COLOR_DARKORANGE 0xFC60 
#define COLOR_DARKORCHID 0x9999 
#define COLOR_DARKRED 0x8800 
#define COLOR_DARKSALMON 0xECAF 
#define COLOR_DARKSEAGREEN 0x8DF1 
#define COLOR_DARKSLATEBLUE 0x49F1 
#define COLOR_DARKSLATEGRAY 0x2A69 
#define COLOR_DARKTURQUOISE 0x067A 
#define COLOR_DARKVIOLET 0x901A 
#define COLOR_DEEPPINK 0xF8B2 
#define COLOR_DEEPSKYBLUE 0x05FF 
#define COLOR_DIMGRAY 0x6B4D 
#define COLOR_DODGERBLUE 0x1C9F 
#define COLOR_FIREBRICK 0xB104 
#define COLOR_FLORALWHITE 0xFFDE 
#define COLOR_FORESTGREEN 0x2444 
#define COLOR_FUCHSIA 0xF81F 
#define COLOR_GAINSBORO 0xDEFB 
#define COLOR_GHOSTWHITE 0xFFDF 
#define COLOR_GOLD 0xFEA0 
#define COLOR_GOLDENROD 0xDD24 
#define COLOR_GRAY 0x8410 
#define COLOR_GREEN 0x0400 
#define COLOR_GREENYELLOW 0xAFE5 
#define COLOR_HONEYDEW 0xF7FE 
#define COLOR_HOTPINK 0xFB56 
#define COLOR_INDIANRED 0xCAEB 
#define COLOR_INDIGO 0x4810 
#define COLOR_IVORY 0xFFFE 
#define COLOR_KHAKI 0xF731 
#define COLOR_LAVENDER 0xE73F 
#define COLOR_LAVENDERBLUSH 0xFF9E 
#define COLOR_LAWNGREEN 0x7FE0 
#define COLOR_LEMONCHIFFON 0xFFD9 
#define COLOR_LIGHTBLUE 0xAEDC 
#define COLOR_LIGHTCORAL 0xF410 
#define COLOR_LIGHTCYAN 0xE7FF 
#define COLOR_LIGHTGOLDENRODYELLOW 0xFFDA 
#define COLOR_LIGHTGREEN 0x9772 
#define COLOR_LIGHTGREY 0xD69A 
#define COLOR_LIGHTPINK 0xFDB8 
#define COLOR_LIGHTSALMON 0xFD0F 
#define COLOR_LIGHTSEAGREEN 0x2595 
#define COLOR_LIGHTSKYBLUE 0x867F 
#define COLOR_LIGHTSLATEGRAY 0x7453 
#define COLOR_LIGHTSTEELBLUE 0xB63B 
#define COLOR_LIGHTYELLOW 0xFFFC 
#define COLOR_LIME 0x07E0 
#define COLOR_LIMEGREEN 0x3666 
#define COLOR_LINEN 0xFF9C 
#define COLOR_MAGENTA 0xF81F 
#define COLOR_MAROON 0x8000 
#define COLOR_MEDIUMAQUAMARINE 0x6675 
#define COLOR_MEDIUMBLUE 0x0019 
#define COLOR_MEDIUMORCHID 0xBABA 
#define COLOR_MEDIUMPURPLE 0x939B 
#define COLOR_MEDIUMSEAGREEN 0x3D8E 
#define COLOR_MEDIUMSLATEBLUE 0x7B5D 
#define COLOR_MEDIUMSPRINGGREEN 0x07D3 
#define COLOR_MEDIUMTURQUOISE 0x4E99 
#define COLOR_MEDIUMVIOLETRED 0xC0B0 
#define COLOR_MIDNIGHTBLUE 0x18CE 
#define COLOR_MINTCREAM 0xF7FF 
#define COLOR_MISTYROSE 0xFF3C 
#define COLOR_MOCCASIN 0xFF36 
#define COLOR_NAVAJOWHITE 0xFEF5 
#define COLOR_NAVY 0x0010 
#define COLOR_OLDLACE 0xFFBC 
#define COLOR_OLIVE 0x8400 
#define COLOR_OLIVEDRAB 0x6C64 
#define COLOR_ORANGE 0xFD20 
#define COLOR_ORANGERED 0xFA20 
#define COLOR_ORCHID 0xDB9A 
#define COLOR_PALEGOLDENROD 0xEF55 
#define COLOR_PALEGREEN 0x9FD3 
#define COLOR_PALETURQUOISE 0xAF7D 
#define COLOR_PALEVIOLETRED 0xDB92 
#define COLOR_PAPAYAWHIP 0xFF7A 
#define COLOR_PEACHPUFF 0xFED7 
#define COLOR_PERU 0xCC27 
#define COLOR_PINK 0xFE19 
#define COLOR_PLUM 0xDD1B 
#define COLOR_POWDERBLUE 0xB71C 
#define COLOR_PURPLE 0x8010 
#define COLOR_RED 0xF800 
#define COLOR_ROSYBROWN 0xBC71 
#define COLOR_ROYALBLUE 0x435C 
#define COLOR_SADDLEBROWN 0x8A22 
#define COLOR_SALMON 0xFC0E 
#define COLOR_SANDYBROWN 0xF52C 
#define COLOR_SEAGREEN 0x2C4A 
#define COLOR_SEASHELL 0xFFBD 
#define COLOR_SIENNA 0xA285 
#define COLOR_SILVER 0xC618 
#define COLOR_SKYBLUE 0x867D 
#define COLOR_SLATEBLUE 0x6AD9 
#define COLOR_SLATEGRAY 0x7412 
#define COLOR_SNOW 0xFFDF 
#define COLOR_SPRINGGREEN 0x07EF 
#define COLOR_STEELBLUE 0x4416 
#define COLOR_TAN 0xD5B1 
#define COLOR_TEAL 0x0410 
#define COLOR_THISTLE 0xDDFB 
#define COLOR_TOMATO 0xFB08 
#define COLOR_TURQUOISE 0x471A 
#define COLOR_VIOLET 0xEC1D 
#define COLOR_WHEAT 0xF6F6 
#define COLOR_WHITE 0xFFFF 
#define COLOR_WHITESMOKE 0xF7BE 
#define COLOR_YELLOW 0xFFE0 
#define COLOR_YELLOWGREEN 0x9E66

#define COLOR_NODEF NULL
//commands
#define VSCRDEF 0x33
#define VSCRSADD 0x37

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

#define SCANLINES 320

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

class TFT9341 {
public:
    TFT9341();
    void InitLCD(byte orientation=LANDSCAPE);
    void setRotation(uint8_t m);
    int  getDisplayXSize();
    int	 getDisplayYSize();
				
    void clrScr();
    void drawRect(int x1, int y1, int x2, int y2, uint16_t color=COLOR_NODEF);
		
    void drawPixel(int x, int y, uint16_t color=NULL);
    void drawLine(int x1, int y1, int x2, int y2, uint16_t color=COLOR_NODEF);
		
    void setColor(uint8_t r, uint8_t g, uint8_t b);
    void setColor(word color);
    uint16_t getColor();
    void setBackColor(uint8_t r, uint8_t g, uint8_t b);
    void setBackColor(uint32_t color);
    uint16_t getBackColor();
    uint16_t getColor(uint8_t r, uint8_t g, uint8_t b);
		
    /**
     * Set text color atributes
     * @param fg WORD, foreground color
     * @param bg WORD, background color
     */
    __attribute__((always_inline)) void setTextColor(uint32_t fg, uint32_t bg) {
			setColor(fg);setBackColor(bg);
    };
 		
    void fillScr(uint8_t r, uint8_t g, uint8_t b);
    /**
     * @desc  Fill all screen with color
     * @param color color number
     */
    void fillScr(uint16_t color);
    
    void fillRect(int x1, int y1, int x2, int y2,  uint16_t color=COLOR_NODEF,  boolean setCS = true);
		
    void drawRoundRect(int x1, int y1, int x2, int y2, uint16_t color=COLOR_NODEF);
    void fillRoundRect(int x1, int y1, int x2, int y2, uint16_t color=COLOR_NODEF);
    /**
     * @desc  Draw circle
     * @param x
     * @param y
     * @param radius
     */
    void drawCircle(int x, int y, int radius, uint16_t color=COLOR_NODEF);
    /**
     * @desc  Draw filled circle
     * @param x
     * @param y
     * @param radius
     */
    void fillCircle(int x, int y, int radius, uint16_t color=COLOR_NODEF);
    
    /**
     * @desc  set font for next print
     * @param font pointer to font
     */
    void setFont(uint8_t* font);
    
    /**
     * @desc  pointer to font
     */
    uint8_t* getFont();
    
    /**
     * @desc  return font X size, for prop. font return 0
     */
    uint8_t getFontXsize();
    
    /**
     * @desc  return font Y size
     */
    uint8_t getFontYsize();
    
    /**
     * @desc  TFT backglight off
     */
    void lcdOff();
    
    /**
     * @desc  TFT backglight ofn
     */
    void lcdOn();
    
    /**
    * @desc  set text area to print
    * @param x
    * @param y
    * @param width
    * @param height
    */
    void setTextArea(int x, int y, int width, int height);
    
    /**
    * @desc  print string array to position
    * @param st string
    * @param x
    * @param y
    */
    void print(String st, int x, int y, int deg=0);
    
    /**
     * @desc  print char array to position
     * @param st string
     * @param x
     * @param y
     */
    void print(char *st, int x, int y, int deg=0);
    
    /**
     * @desc  print string into area defined with setTextArea.
     *        Increment line after print
     * @param st string
     */
    void print(char *st);
    
    /**
     * @desc  print string into area defined with setTextArea.
     *        Increment line after print
     * @param st string
     */
    void print(String st);
    
    /**
     * @desc  print string into area defined with setTextArea. 
     *        Increment line after print
     * @param st string
     */
    void println(String st);
    
    /**
     * @desc  print char array into area defined with setTextArea.
     *        Increment line after print
     * @param st string
     */
    void println(char *st);
    
    /**
     * @desc  print number into area defined with setTextArea.
     *        Increment line after print
     * @param st string
     */
        void println(long n);
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
    uint16_t scanline[SCANLINES*2];
    uint16_t _fgc, _bgc;
    uint16_t _x, _y;
    uint16_t _xmin = 0;
    uint16_t _ymin = 0;
    uint16_t _xmax, _ymax ;

    void setXY(int x1, int y1, int x2, int y2);
    void drawHLine(int x, int y, int l, uint16_t color = NULL);
    void drawHLine_noCS(int x, int y, int l,uint16_t color = NULL);
    void drawVLine(int x, int y, int l, uint16_t color = NULL);
    void drawVLine_noCS(int x, int y, int l, uint16_t color = NULL);
    void drawPixel_noCS(int x, int y, uint16_t color = NULL);
    void setPixel(uint16_t color);
    void printChar(byte c, int x, int y);
      
    static uint16_t color565(uint8_t r, uint8_t g, uint8_t b) {
        return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
    }
  
};

extern TFT9341 Tft;

#endif
