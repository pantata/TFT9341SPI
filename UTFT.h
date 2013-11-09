/*
 ILI9341 2.2 TFT SPI library
 based on UTFT.cpp - Arduino/chipKit library support for Color TFT LCD Boards
 Copyright (C)2010-2013 Henning Karlsen. All right reserved
 
 Compatible with other UTFT libraries.
 
 Original library you can find at http://electronics.henningkarlsen.com/
  
 This library is free software; you can redistribute it and/or
 modify it under the terms of the CC BY-NC-SA 3.0 license.
 Please see the included documents for further information.
 */

#ifndef UTFT_h
#define UTFT_h

#define UTFT_VERSION	270

#define LEFT 0
#define RIGHT 9999
#define CENTER 9998

#define PORTRAIT 0
#define LANDSCAPE 1

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

#include "Arduino.h"
//#include "HW_AVR_defines.h"

#ifndef INT8U
#define INT8U unsigned char
#endif
#ifndef INT16U
#define INT16U unsigned int
#endif

#define bitmapdatatype unsigned int*
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

class UTFT
{
	public:
		UTFT();
		void InitLCD(byte orientation=LANDSCAPE);
		void clrScr();
		void drawPixel(int x, int y);
		void drawLine(int x1, int y1, int x2, int y2);
		void fillScr(byte r, byte g, byte b);
		void fillScr(word color);
		void drawRect(int x1, int y1, int x2, int y2);
		void drawRoundRect(int x1, int y1, int x2, int y2);
		void fillRect(int x1, int y1, int x2, int y2);
		void fillRoundRect(int x1, int y1, int x2, int y2);
		void drawCircle(int x, int y, int radius);
		void fillCircle(int x, int y, int radius);
		void setColor(byte r, byte g, byte b);
		void setColor(word color);
		word getColor();
		void setBackColor(byte r, byte g, byte b);
		void setBackColor(uint32_t color);
		word getBackColor();
		void print(char *st, int x, int y, int deg=0);
		void print(String st, int x, int y, int deg=0);
		void printNumI(long num, int x, int y, int length=0, char filler=' ');
		void printNumF(double num, byte dec, int x, int y, char divider='.', int length=0, char filler=' ');
		void setFont(uint8_t* font);
		uint8_t* getFont();
		uint8_t getFontXsize();
		uint8_t getFontYsize();
		void drawBitmap(int x, int y, int sx, int sy, bitmapdatatype data, int scale=1);
		void drawBitmap(int x, int y, int sx, int sy, bitmapdatatype data, int deg, int rox, int roy);
		void lcdOff();
		void lcdOn();
		void setContrast(char c);
		int  getDisplayXSize();
		int	 getDisplayYSize();

/*
	The functions and variables below should not normally be used.
	They have been left publicly available for use in add-on libraries
	that might need access to the lower level functions of UTFT.

	Please note that these functions and variables are not documented
	and I do not provide support on how to use them.
*/
		byte fch, fcl, bch, bcl;
		byte orient;
		long disp_x_size, disp_y_size;
		//byte display_model, display_transfer_mode, display_serial_mode;
		//regtype *P_RS, *P_WR, *P_CS, *P_RST, *P_SDA, *P_SCL, *P_ALE;
		//regsize B_RS, B_WR, B_CS, B_RST, B_SDA, B_SCL, B_ALE;
		_current_font	cfont;
		boolean _transparent;

		//void LCD_Writ_Bus(char VH,char VL, byte mode);
		void LCD_Write_COM(char VL);
		void LCD_Write_DATA(char VH,char VL);
		void LCD_Write_DATA(char VL);
		void LCD_Write_COM_DATA(char com1,int dat1);
		//void _hw_special_init();
		void setPixel(word color);
		void drawHLine(int x, int y, int l);
		void drawVLine(int x, int y, int l);
		void printChar(byte c, int x, int y);
		void setXY(word x1, word y1, word x2, word y2);
		void clrXY();
		void rotateChar(byte c, int x, int y, int pos, int deg);
		//void _set_direction_registers(byte mode);
		//void _fast_fill_16(int ch, int cl, long pix);
		//void _fast_fill_8(int ch, long pix);
		void _convert_float(char *buf, double num, int width, byte prec);
        void sendCMD(INT8U VL);
        void WRITE_DATA(INT8U VL);
        void WRITE_Package(INT16U *data, INT8U howmany);
        void sendData(INT16U data);
        INT8U readID(void);
    INT8U Read_Register(INT8U Addr, INT8U xParameter);
};

#endif