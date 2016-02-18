/*
  ILI9341 2.2 TFT SPI library
  based on UTFT.cpp 
  
  Compatible with other UTFT libraries.
  Original library you can find at http://electronics.henningkarlsen.com/
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the CC BY-NC-SA 3.0 license.
  Please see the included documents for further information.
*/


#include "Arduino.h"
#include "TFT9341.h"
#include <pins_arduino.h>
#include <SPI.h>

//dma test

	
#if defined(__AVR__)

	#include <digitalWriteFast.h>
	
	#define TFT_BL_OFF digitalWriteFast(LED,LOW) 
	#define TFT_BL_ON  digitalWriteFast(LED,HIGH)

	#define TFT_RST_OFF digitalWriteFast(RESET, HIGH) 
	#define TFT_RST_ON  digitalWriteFast(RESET,LOW) 


	#define TFT_CS_LOW digitalWriteFast(CS,LOW)
	#define TFT_CS_HIGH digitalWriteFast(CS,HIGH) 

	#define TFT_DC_LOW  digitalWriteFast(DC,LOW) 
	#define TFT_DC_HIGH digitalWriteFast(DC,HIGH) 
	#define PINMODE(x,y) pinModeFast(x,y)
	
	#define SPI_TRANSFER(x) SPI.transfer(x)
	
#elif defined(__arm__)
	    
	#define PINMODE(x,y) pinMode(x,y)  
	//#if defined(SPI_MODE_FAST)	 
	#define SPI_TRANSFER(x) SPI.transfer(x)
	//#elif defined(SPI_MODE_DMA)
	//	#define SPI_TRANSFER(x) transfer(x);
	//#endif	
	
	volatile uint32_t *setCSPin = &PORT->Group[g_APinDescription[CS].ulPort].OUTSET.reg;
	volatile uint32_t *clrCSPin = &PORT->Group[g_APinDescription[CS].ulPort].OUTCLR.reg;
	
	volatile uint32_t *setDCPin = &PORT->Group[g_APinDescription[DC].ulPort].OUTSET.reg;
	volatile uint32_t *clrDCPin = &PORT->Group[g_APinDescription[DC].ulPort].OUTCLR.reg;

	volatile uint32_t *setLEDPin = &PORT->Group[g_APinDescription[LED].ulPort].OUTSET.reg;
	volatile uint32_t *clrLEDPin = &PORT->Group[g_APinDescription[LED].ulPort].OUTCLR.reg;

	volatile uint32_t *setRSTPin = &PORT->Group[g_APinDescription[RESET].ulPort].OUTSET.reg;
	volatile uint32_t *clrRSTPin = &PORT->Group[g_APinDescription[RESET].ulPort].OUTCLR.reg;

	#define TFT_CS_LOW  *clrCSPin = (1ul << g_APinDescription[CS].ulPin)
	#define TFT_CS_HIGH  *setCSPin = (1ul << g_APinDescription[CS].ulPin)

	#define TFT_DC_LOW   *clrDCPin = (1ul << g_APinDescription[DC].ulPin)
	#define TFT_DC_HIGH  *setDCPin = (1ul << g_APinDescription[DC].ulPin)

	#define TFT_RST_OFF *setRSTPin = (1ul << g_APinDescription[RESET].ulPin)
	#define TFT_RST_ON  *clrRSTPin = (1ul << g_APinDescription[RESET].ulPin)

	#define TFT_BL_OFF *clrLEDPin = (1ul << g_APinDescription[LED].ulPin)
	#define TFT_BL_ON  *setLEDPin = (1ul << g_APinDescription[LED].ulPin)					
#endif

TFT9341::TFT9341() {}


void TFT9341::LCD_Write_DATA(char VH,char VL)
{
    TFT_DC_HIGH;
    TFT_CS_LOW;      
    SPI_TRANSFER(VH);
    SPI_TRANSFER(VL);
	TFT_CS_HIGH;    
  
}

INT8U TFT9341::Read_Register(INT8U Addr, INT8U xParameter)
{
    INT8U data=0;
    TFT_DC_LOW;
    TFT_CS_LOW;    
    SPI_TRANSFER(0xd9);      
    TFT_DC_HIGH;    
    SPI_TRANSFER(0x10+xParameter);  
    TFT_DC_LOW;
    SPI_TRANSFER(Addr);
    TFT_DC_HIGH;
    data = SPI_TRANSFER(0);
    TFT_CS_HIGH;
    return data;
}

INT8U TFT9341::readID(void)
{
    INT8U i=0;
    INT8U data[3] ;
    INT8U ID[3] = {0x00, 0x93, 0x41};
    INT8U ToF=1;
    for(i=0;i<3;i++)
    {
        data[i]=Read_Register(0xd3,i+1);
        if(data[i] != ID[i])
        {
            ToF=0;
        }
    }
    delay(10);
    return ToF;
}

void TFT9341::InitLCD(byte orientation)
{
	
	
	PINMODE(LED,OUTPUT);
	PINMODE(RESET,OUTPUT);
	PINMODE(CS,OUTPUT);
	PINMODE(DC,OUTPUT);
    
    TFT_BL_ON;

#if defined(__AVR__)
	SPI.begin();
	#if defined(SPI_MODE_FAST)
		SPI.setClockDivider( SPI_CLOCK_DIV2 );	
	#endif	
#elif defined(__arm__)
	SPI.begin();
	SPI.setClockDivider( 2 );				
#endif  
      
    TFT_CS_HIGH;
    TFT_DC_HIGH;

    TFT_RST_OFF;
    delay(5);
	TFT_RST_ON;
	delay(20);
	TFT_RST_OFF;
    delay(100);
    
	TFT_DC_LOW;
    TFT_CS_LOW;
    SPI_TRANSFER(0xCB);
    TFT_DC_HIGH;	
    SPI_TRANSFER(0x39);
    SPI_TRANSFER(0x2C);
    SPI_TRANSFER(0x00);
    SPI_TRANSFER(0x34);
    SPI_TRANSFER(0x02);        
    
	TFT_DC_LOW;    
	SPI_TRANSFER(0xCF);
    TFT_DC_HIGH;	
	SPI_TRANSFER(0x00);
	SPI_TRANSFER(0XC1);
	SPI_TRANSFER(0X30);

	TFT_DC_LOW;     
	SPI_TRANSFER(0xE8);
	TFT_DC_HIGH;
	SPI_TRANSFER(0x85);
	SPI_TRANSFER(0x00);
	SPI_TRANSFER(0x78);
    
    TFT_DC_LOW;     
	SPI_TRANSFER(0xEA);
    TFT_DC_HIGH; 	
	SPI_TRANSFER(0x00);
	SPI_TRANSFER(0x00);
    
    TFT_DC_LOW; 
	SPI_TRANSFER(0xED);
	TFT_DC_HIGH;	
	SPI_TRANSFER(0x64);
	SPI_TRANSFER(0x03);
	SPI_TRANSFER(0X12);
	SPI_TRANSFER(0X81);
    
    TFT_DC_LOW; 
	SPI_TRANSFER(0xF7);
	TFT_DC_HIGH;		
	SPI_TRANSFER(0x20);
    
    TFT_DC_LOW;
	SPI_TRANSFER(0xC0);    	
	TFT_DC_HIGH;
	SPI_TRANSFER(0x23);   	
    
    TFT_DC_LOW;
	SPI_TRANSFER(0xC1);    	
	TFT_DC_HIGH;
	SPI_TRANSFER(0x10);   	
    
    TFT_DC_LOW;
	SPI_TRANSFER(0xC5);    	
	TFT_DC_HIGH;
	SPI_TRANSFER(0x3e);   	
	SPI_TRANSFER(0x28);
    
    TFT_DC_LOW;    
	SPI_TRANSFER(0xC7);    	
	TFT_DC_HIGH;	
	SPI_TRANSFER(0x86);  	

    TFT_DC_LOW;    
	SPI_TRANSFER(0x36);    	
	TFT_DC_HIGH;	
	SPI_TRANSFER(0x48);  	

    TFT_DC_LOW;    
	SPI_TRANSFER(0x3A);
	TFT_DC_HIGH;	
	SPI_TRANSFER(0x55);

    TFT_DC_LOW;    
	SPI_TRANSFER(0xB1);
	TFT_DC_HIGH;	
	SPI_TRANSFER(0x00);
	SPI_TRANSFER(0x18);

    TFT_DC_LOW;    
	SPI_TRANSFER(0xB6);    	// Display Function Control
	TFT_DC_HIGH;	
	SPI_TRANSFER(0x08);
	SPI_TRANSFER(0x82);
	SPI_TRANSFER(0x27);

    TFT_DC_LOW;    
	SPI_TRANSFER(0xF2);    	// 3Gamma Function Disable
	TFT_DC_HIGH;	
	SPI_TRANSFER(0x00);

    TFT_DC_LOW;    
	SPI_TRANSFER(0x26);    	//Gamma curve selected
	TFT_DC_HIGH;	
	SPI_TRANSFER(0x01);

    TFT_DC_LOW;    
	SPI_TRANSFER(0xE0);    	//Set Gamma
	TFT_DC_HIGH;	
	SPI_TRANSFER(0x0F);
	SPI_TRANSFER(0x31);
	SPI_TRANSFER(0x2B);
	SPI_TRANSFER(0x0C);
	SPI_TRANSFER(0x0E);
	SPI_TRANSFER(0x08);
	SPI_TRANSFER(0x4E);
	SPI_TRANSFER(0xF1);
	SPI_TRANSFER(0x37);
	SPI_TRANSFER(0x07);
	SPI_TRANSFER(0x10);
	SPI_TRANSFER(0x03);
	SPI_TRANSFER(0x0E);
	SPI_TRANSFER(0x09);
	SPI_TRANSFER(0x00);

    TFT_DC_LOW;    
	SPI_TRANSFER(0XE1);    	//Set Gamma
	TFT_DC_HIGH;	
	SPI_TRANSFER(0x00);
	SPI_TRANSFER(0x0E);
	SPI_TRANSFER(0x14);
	SPI_TRANSFER(0x03);
	SPI_TRANSFER(0x11);
	SPI_TRANSFER(0x07);
	SPI_TRANSFER(0x31);
	SPI_TRANSFER(0xC1);
	SPI_TRANSFER(0x48);
	SPI_TRANSFER(0x08);
	SPI_TRANSFER(0x0F);
	SPI_TRANSFER(0x0C);
	SPI_TRANSFER(0x31);
	SPI_TRANSFER(0x36);
	SPI_TRANSFER(0x0F);

    TFT_DC_LOW;    
	SPI_TRANSFER(0x11);    	//Exit Sleep
	delay(120);

	SPI_TRANSFER(0x29);    //Display on 
	SPI_TRANSFER(0x2c);
	TFT_CS_HIGH;		
    
	cfont.font=0;
	_transparent = false;
	
	setRotation(orientation);
}

void TFT9341::setXY(word x1, word y1, word x2, word y2)
{
	int tmp;
/*
	if (orient==LANDSCAPE)
	{
		swap(word, x1, y1);
		swap(word, x2, y2)
		y1=disp_y_size-y1;
		y2=disp_y_size-y2;
		swap(word, y1, y2)
	}
*/
    TFT_DC_LOW;
    TFT_CS_LOW;
    SPI_TRANSFER(0x2a);
    TFT_DC_HIGH;
	SPI_TRANSFER(x1>>8);
	SPI_TRANSFER(x1);
	SPI_TRANSFER(x2>>8);
	SPI_TRANSFER(x2);
    TFT_DC_LOW;
    SPI_TRANSFER(0x2b);
    TFT_DC_HIGH;
	SPI_TRANSFER(y1>>8);
	SPI_TRANSFER(y1);
	SPI_TRANSFER(y2>>8);
	SPI_TRANSFER(y2);
	TFT_DC_LOW;
    SPI_TRANSFER(0x2c);
    TFT_DC_HIGH;
    TFT_CS_HIGH;
}

void TFT9341::clrXY()
{
	//if (orient==PORTRAIT)
		setXY(0,0,disp_x_size,disp_y_size);
	//else
	//	setXY(0,0,disp_y_size,disp_x_size);
}

void TFT9341::drawRect(int x1, int y1, int x2, int y2)
{
	int tmp;

	if (x1>x2)
	{
		swap(int, x1, x2);
	}
	if (y1>y2)
	{
		swap(int, y1, y2);
	}

	drawHLine(x1, y1, x2-x1);
	drawHLine(x1, y2, x2-x1);
	drawVLine(x1, y1, y2-y1);
	drawVLine(x2, y1, y2-y1);
}


void TFT9341::fillRect(int x1, int y1, int x2, int y2)
{
	int tmp;

	if (x1>x2)
	{
		swap(int, x1, x2);
	}
	if (y1>y2)
	{
		swap(int, y1, y2);
	}

		//if (orient==PORTRAIT)
		//{
			for (int i=0; i<((y2-y1)/2)+1; i++)
			{
				drawHLine(x1, y1+i, x2-x1);
				drawHLine(x1, y2-i, x2-x1);
			}
		/*}
		else
		{
			for (int i=0; i<((x2-x1)/2)+1; i++)
			{
				drawVLine(x1+i, y1, y2-y1);
				drawVLine(x2-i, y1, y2-y1);
			}
		}*/
	}



void TFT9341::drawRoundRect(int x1, int y1, int x2, int y2)
{
    int tmp;
    
    if (x1>x2)
    {
        swap(int, x1, x2);
    }
    if (y1>y2)
    {
        swap(int, y1, y2);
    }
    if ((x2-x1)>4 && (y2-y1)>4)
    {
        drawPixel(x1+1,y1+1);
        drawPixel(x2-1,y1+1);
        drawPixel(x1+1,y2-1);
        drawPixel(x2-1,y2-1);
        drawHLine(x1+2, y1, x2-x1-4);
        drawHLine(x1+2, y2, x2-x1-4);
        drawVLine(x1, y1+2, y2-y1-4);
        drawVLine(x2, y1+2, y2-y1-4);
    }
}

void TFT9341::fillRoundRect(int x1, int y1, int x2, int y2)
{
	int tmp;

	if (x1>x2)
	{
		swap(int, x1, x2);
	}
	if (y1>y2)
	{
		swap(int, y1, y2);
	}

	if ((x2-x1)>4 && (y2-y1)>4)
	{
		for (int i=0; i<((y2-y1)/2)+1; i++)
		{
			switch(i)
			{
			case 0:
				drawHLine(x1+2, y1+i, x2-x1-4);
				drawHLine(x1+2, y2-i, x2-x1-4);
				break;
			case 1:
				drawHLine(x1+1, y1+i, x2-x1-2);
				drawHLine(x1+1, y2-i, x2-x1-2);
				break;
			default:
				drawHLine(x1, y1+i, x2-x1);
				drawHLine(x1, y2-i, x2-x1);
			}
		}
	}
}

void TFT9341::drawCircle(int x, int y, int radius)
{
	int f = 1 - radius;
	int ddF_x = 1;
	int ddF_y = -2 * radius;
	int x1 = 0;
	int y1 = radius;
 
	setXY(x, y + radius, x, y + radius);
	LCD_Write_DATA(fch,fcl);
	setXY(x, y - radius, x, y - radius);
	LCD_Write_DATA(fch,fcl);
	setXY(x + radius, y, x + radius, y);
	LCD_Write_DATA(fch,fcl);
	setXY(x - radius, y, x - radius, y);
	LCD_Write_DATA(fch,fcl);

	while(x1 < y1)
	{
		if(f >= 0) 
		{
			y1--;
			ddF_y += 2;
			f += ddF_y;
		}
		x1++;
		ddF_x += 2;
		f += ddF_x;    
		setXY(x + x1, y + y1, x + x1, y + y1);
		LCD_Write_DATA(fch,fcl);
		setXY(x - x1, y + y1, x - x1, y + y1);
		LCD_Write_DATA(fch,fcl);
		setXY(x + x1, y - y1, x + x1, y - y1);
		LCD_Write_DATA(fch,fcl);
		setXY(x - x1, y - y1, x - x1, y - y1);
		LCD_Write_DATA(fch,fcl);
		setXY(x + y1, y + x1, x + y1, y + x1);
		LCD_Write_DATA(fch,fcl);
		setXY(x - y1, y + x1, x - y1, y + x1);
		LCD_Write_DATA(fch,fcl);
		setXY(x + y1, y - x1, x + y1, y - x1);
		LCD_Write_DATA(fch,fcl);
		setXY(x - y1, y - x1, x - y1, y - x1);
		LCD_Write_DATA(fch,fcl);
	}
	clrXY();
}

void TFT9341::fillCircle(int x, int y, int radius)
{
	for(int y1=-radius; y1<=0; y1++) 
		for(int x1=-radius; x1<=0; x1++)
			if(x1*x1+y1*y1 <= radius*radius) 
			{
				drawHLine(x+x1, y+y1, 2*(-x1));
				drawHLine(x+x1, y-y1, 2*(-x1));
				break;
			}
}


void TFT9341::clrScr()
{
	long i;
      
    clrXY();
    TFT_CS_LOW;	
    TFT_DC_HIGH;
  
    for (int i=0; i<((disp_x_size+1)*(disp_y_size+1)); i++)
    {
        // black
	    SPI_TRANSFER(0);
    	SPI_TRANSFER(0);
    }
    
     TFT_CS_HIGH;
}

void TFT9341::fillScr(byte r, byte g, byte b)
{
	word color = ((r&248)<<8 | (g&252)<<3 | (b&248)>>3);
	fillScr(color);
}

void TFT9341::fillScr(word color)
{
	long i;
	char ch, cl;
	
	ch=byte(color>>8);
	cl=byte(color & 0xFF);
	    
    clrXY();
    
    TFT_CS_LOW;
    TFT_DC_HIGH;

    for (int i=0; i<((disp_x_size+1)*(disp_y_size+1)); i++)
    {
	    SPI_TRANSFER(ch);
    	SPI_TRANSFER(cl);
    }
    TFT_CS_HIGH;
}

void TFT9341::setColor(byte r, byte g, byte b)
{
	fch=((r&248)|g>>5);
	fcl=((g&28)<<3|b>>3);
}

void TFT9341::setColor(word color)
{
	fch=byte(color>>8);
	fcl=byte(color & 0xFF);
}

word TFT9341::getColor()
{
	return (fch<<8) | fcl;
}

void TFT9341::setBackColor(byte r, byte g, byte b)
{
	bch=((r&248)|g>>5);
	bcl=((g&28)<<3|b>>3);
	_transparent=false;
}

void TFT9341::setBackColor(uint32_t color)
{
	if (color==VGA_TRANSPARENT)
		_transparent=true;
	else
	{
		bch=byte(color>>8);
		bcl=byte(color & 0xFF);
		_transparent=false;
	}
}

word TFT9341::getBackColor()
{
	return (bch<<8) | bcl;
}

void TFT9341::setPixel(word color)
{
	LCD_Write_DATA((color>>8),(color&0xFF));	// rrrrrggggggbbbbb
}

void TFT9341::drawPixel(int x, int y)
{
	setXY(x, y, x, y);
	setPixel((fch<<8)|fcl);
	clrXY();
}

void TFT9341::drawLine(int x1, int y1, int x2, int y2)
{
	if (y1==y2)
		drawHLine(x1, y1, x2-x1);
	else if (x1==x2)
		drawVLine(x1, y1, y2-y1);
	else
	{
		unsigned int	dx = (x2 > x1 ? x2 - x1 : x1 - x2);
		short			xstep =  x2 > x1 ? 1 : -1;
		unsigned int	dy = (y2 > y1 ? y2 - y1 : y1 - y2);
		short			ystep =  y2 > y1 ? 1 : -1;
		int				col = x1, row = y1;

		if (dx < dy)
		{
			int t = - (dy >> 1);
			while (true)
			{
				setXY (col, row, col, row);
				LCD_Write_DATA (fch, fcl);TFT_CS_HIGH;
				if (row == y2)
					return;
				row += ystep;
				t += dx;
				if (t >= 0)
				{
					col += xstep;
					t   -= dy;
				}
			} 
		}
		else
		{
			int t = - (dx >> 1);
			while (true)
			{
				setXY (col, row, col, row);
				LCD_Write_DATA (fch, fcl);
				if (col == x2)
					return;
				col += xstep;
				t += dy;
				if (t >= 0)
				{
					row += ystep;
					t   -= dx;
				}
			} 
		}
	}
	clrXY();
}

void TFT9341::drawHLine(int x, int y, int l)
{
	if (l<0)
	{
		l = -l;
		x -= l;
	}
	setXY(x, y, x+l, y);
	TFT_DC_HIGH;
    TFT_CS_LOW;
		for (int i=0; i<l+1; i++)
		{
			SPI_TRANSFER(fch);
			SPI_TRANSFER(fcl);
		}

	clrXY();
}

void TFT9341::drawVLine(int x, int y, int l)
{
	if (l<0)
	{
		l = -l;
		y -= l;
	}

	setXY(x, y, x, y+l);

    TFT_DC_HIGH;
    TFT_CS_LOW;
		for (int i=0; i<l+1; i++)
		{
			SPI_TRANSFER(fch);
			SPI_TRANSFER(fcl);
		}

	clrXY();	
}

void TFT9341::printChar(byte c, int x, int y)
{
	byte i,ch;
	word j;
	word temp; 
  
	if (!_transparent)
	{
		//if (orient==PORTRAIT)
		//{
			setXY(x,y,x+cfont.x_size-1,y+cfont.y_size-1);
	  
			temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;
			for(j=0;j<((cfont.x_size/8)*cfont.y_size);j++)
			{
				ch=pgm_read_byte(&cfont.font[temp]);
				for(i=0;i<8;i++)
				{   
					if((ch&(1<<(7-i)))!=0)   
					{
						setPixel((fch<<8)|fcl);
					} 
					else
					{
						setPixel((bch<<8)|bcl);
					}   
				}
				temp++;
			}
		/*}
		else
		{
			temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;

			for(j=0;j<((cfont.x_size/8)*cfont.y_size);j+=(cfont.x_size/8))
			{
				setXY(x,y+(j/(cfont.x_size/8)),x+cfont.x_size-1,y+(j/(cfont.x_size/8)));
				for (int zz=(cfont.x_size/8)-1; zz>=0; zz--)
				{
					ch=pgm_read_byte(&cfont.font[temp+zz]);
					for(i=0;i<8;i++)
					{   
						if((ch&(1<<i))!=0)   
						{
							setPixel((fch<<8)|fcl);
						} 
						else
						{
							setPixel((bch<<8)|bcl);
						}   
					}
				}
				temp+=(cfont.x_size/8);
			}
		}*/
	}
	else
	{
		temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;
		for(j=0;j<cfont.y_size;j++) 
		{
			for (int zz=0; zz<(cfont.x_size/8); zz++)
			{
				ch=pgm_read_byte(&cfont.font[temp+zz]); 
				for(i=0;i<8;i++)
				{   
					setXY(x+i+(zz*8),y+j,x+i+(zz*8)+1,y+j+1);
				
					if((ch&(1<<(7-i)))!=0)   
					{
						setPixel((fch<<8)|fcl);
					} 
				}
			}
			temp+=(cfont.x_size/8);
		}
	}


	clrXY();
}

void TFT9341::rotateChar(byte c, int x, int y, int pos, int deg)
{
 	byte i,j,ch;
	word temp; 
	int newx,newy;
	double radian;
	radian=deg*0.0175;  
	temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;
	for(j=0;j<cfont.y_size;j++) 
	{
		for (int zz=0; zz<(cfont.x_size/8); zz++)
		{
			ch=pgm_read_byte(&cfont.font[temp+zz]); 
			for(i=0;i<8;i++)
			{   
				newx=x+(((i+(zz*8)+(pos*cfont.x_size))*cos(radian))-((j)*sin(radian)));
				newy=y+(((j)*cos(radian))+((i+(zz*8)+(pos*cfont.x_size))*sin(radian)));

				setXY(newx,newy,newx+1,newy+1);
				
				if((ch&(1<<(7-i)))!=0)   
				{
					setPixel((fch<<8)|fcl);
				} 
				else  
				{
					if (!_transparent)
						setPixel((bch<<8)|bcl);
				}   
			}
		}
		temp+=(cfont.x_size/8);
	}
	clrXY();
}
/*
void TFT9341::print(char *st, int x, int y, int deg)
{
	int stl, i;

	stl = strlen(st);

	if (orient==PORTRAIT)
	{
	if (x==RIGHT)
		x=(disp_x_size+1)-(stl*cfont.x_size);
	if (x==CENTER)
		x=((disp_x_size+1)-(stl*cfont.x_size))/2;
	}
	else
	{
	if (x==RIGHT)
		x=(disp_y_size+1)-(stl*cfont.x_size);
	if (x==CENTER)
		x=((disp_y_size+1)-(stl*cfont.x_size))/2;
	}

	for (i=0; i<stl; i++)
		if (deg==0)
			printChar(*st++, x + (i*(cfont.x_size)), y);
		else
			rotateChar(*st++, x, y, i, deg);
}

*/
void TFT9341::print(String st, int x, int y, int deg)
{
	char buf[st.length()+1];
    st.toCharArray(buf, st.length()+1);
	print(buf, x, y, deg);
}

void TFT9341::printNumI(long num, int x, int y, int length, char filler)
{
	char buf[25];
	char st[27];
	boolean neg=false;
	int c=0, f=0;
  
	if (num==0)
	{
		if (length!=0)
		{
			for (c=0; c<(length-1); c++)
				st[c]=filler;
			st[c]=48;
			st[c+1]=0;
		}
		else
		{
			st[0]=48;
			st[1]=0;
		}
	}
	else
	{
		if (num<0)
		{
			neg=true;
			num=-num;
		}
	  
		while (num>0)
		{
			buf[c]=48+(num % 10);
			c++;
			num=(num-(num % 10))/10;
		}
		buf[c]=0;
	  
		if (neg)
		{
			st[0]=45;
		}
	  
		if (length>(c+neg))
		{
			for (int i=0; i<(length-c-neg); i++)
			{
				st[i+neg]=filler;
				f++;
			}
		}

		for (int i=0; i<c; i++)
		{
			st[i+neg+f]=buf[c-i-1];
		}
		st[c+neg+f]=0;

	}

	print(st,x,y);
}
/*
void TFT9341::printNumF(double num, byte dec, int x, int y, char divider, int length, char filler)
{
	char st[27];
	boolean neg=false;

	if (dec<1)
		dec=1;
	else if (dec>5)
		dec=5;

	if (num<0)
		neg = true;

	_convert_float(st, num, length, dec);

	if (divider != '.')
	{
		for (int i=0; i<sizeof(st); i++)
			if (st[i]=='.')
				st[i]=divider;
	}

	if (filler != ' ')
	{
		if (neg)
		{
			st[0]='-';
			for (int i=1; i<sizeof(st); i++)
				if ((st[i]==' ') || (st[i]=='-'))
					st[i]=filler;
		}
		else
		{
			for (int i=0; i<sizeof(st); i++)
				if (st[i]==' ')
					st[i]=filler;
		}
	}

	print(st,x,y);
}
*/
void TFT9341::setFont(uint8_t* font)
{
	cfont.font=font;
	cfont.x_size=fontbyte(0);
	cfont.y_size=fontbyte(1);
	cfont.offset=fontbyte(2);
	cfont.numchars=fontbyte(3);
}

uint8_t* TFT9341::getFont()
{
	return cfont.font;
}

uint8_t TFT9341::getFontXsize()
{
	return cfont.x_size;
}

uint8_t TFT9341::getFontYsize()
{
	return cfont.y_size;
}

void TFT9341::drawBitmap(int x, int y, int sx, int sy, bitmapdatatype data, int scale)
{
	unsigned int col;
	int tx, ty, tc, tsx, tsy;
	byte r, g, b;
	
	if (scale==1)
	{
		//if (orient==PORTRAIT)
		//{

			setXY(x, y, x+sx-1, y+sy-1);
			for (tc=0; tc<(sx*sy); tc++)
			{
				col=pgm_read_word(&data[tc]);
				LCD_Write_DATA(col>>8,col & 0xff);
			}
					
		/*} else
		{
			for (ty=0; ty<sy; ty++)
			{
				setXY(x, y+ty, x+sx-1, y+ty);
				for (tx=sx-1; tx>=0; tx--)
				{
					col=pgm_read_word(&data[(ty*sx)+tx]);
					LCD_Write_DATA(col>>8,col & 0xff);
				}
			}

		}*/
	}
	else
	{
		//if (orient==PORTRAIT)
		//{

			for (ty=0; ty<sy; ty++)
			{
				setXY(x, y+(ty*scale), x+((sx*scale)-1), y+(ty*scale)+scale);
				for (tsy=0; tsy<scale; tsy++) {
					for (tx=0; tx<sx; tx++) {
						col=pgm_read_word(&data[(ty*sx)+tx]);
						for (tsx=0; tsx<scale; tsx++)
							LCD_Write_DATA(col>>8,col & 0xff);
					}
				}
			}

		/*}
		else
		{

			for (ty=0; ty<sy; ty++)
			{
				for (tsy=0; tsy<scale; tsy++)
				{
					setXY(x, y+(ty*scale)+tsy, x+((sx*scale)-1), y+(ty*scale)+tsy);
					for (tx=sx-1; tx>=0; tx--)
					{
						col=pgm_read_word(&data[(ty*sx)+tx]);
						for (tsx=0; tsx<scale; tsx++)
							LCD_Write_DATA(col>>8,col & 0xff);
					}
				}
			}

		}*/
	}
	clrXY();
}

void TFT9341::drawBitmap(int x, int y, int sx, int sy, bitmapdatatype data, int deg, int rox, int roy)
{
	unsigned int col;
	int tx, ty, newx, newy;
	byte r, g, b;
	double radian;
	radian=deg*0.0175;  
    
	if (deg==0)
		drawBitmap(x, y, sx, sy, data);
	else
	{
			for (ty=0; ty<sy; ty++)
			for (tx=0; tx<sx; tx++)
			{
				col=pgm_read_word(&data[(ty*sx)+tx]);

				newx=x+rox+(((tx-rox)*cos(radian))-((ty-roy)*sin(radian)));
				newy=y+roy+(((ty-roy)*cos(radian))+((tx-rox)*sin(radian)));

				setXY(newx, newy, newx, newy);
				LCD_Write_DATA(col>>8,col & 0xff);
			}

	}
	clrXY();
	
}

void TFT9341::lcdOff()
{
    TFT_BL_OFF;
}

void TFT9341::lcdOn()
{
    TFT_BL_ON;
}

/*
void TFT9341::setContrast(char c)
{

}
*/

int TFT9341::getDisplayXSize()
{
	
	return disp_x_size+1;
}

int TFT9341::getDisplayYSize()
{
	return disp_y_size+1;
}

#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04

void TFT9341::setRotation(uint8_t m) {
    TFT_DC_LOW;
    TFT_CS_LOW;
    SPI_TRANSFER(0x36);
    TFT_DC_HIGH;
    rotation = m % 4; // can't be higher than 3
    switch (rotation) {
        case 0:
            SPI_TRANSFER(MADCTL_MX | MADCTL_BGR);
            orient=PORTRAIT;
            disp_x_size=239;
		    disp_y_size=319;            
            break;
        case 1:
            SPI_TRANSFER(MADCTL_MV | MADCTL_BGR);
            orient=LANDSCAPE;
            disp_y_size=239;
		    disp_x_size=319;            
            break;
        case 2:
            SPI_TRANSFER(MADCTL_MY | MADCTL_BGR);
            orient=PORTRAIT;
            disp_x_size=239;
		    disp_y_size=319;            
            break;
        case 3:
            SPI_TRANSFER(MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
            orient=LANDSCAPE;
            disp_y_size=239;
		    disp_x_size=319;            
            break;
    }
    TFT_CS_HIGH;
}

#define swap(a, b) { int16_t t = a; a = b; b = t; }
void TFT9341::drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
	drawLine(x1, y1, x2, y2);
	drawLine(x2, y2, x3, y3);
	drawLine(x3, y3, x1, y1);
}

void TFT9341::fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
	int32_t xs, xe;
	int16_t y, ly;

	if (y1 > y2)
	{
		swap(y1, y2); 
		swap(x1, x2);
	}
	if (y2 > y3)
	{
		swap(y3, y2);
		swap(x3, x2);
	}
	if (y1 > y2)
	{
		swap(y1, y2);
		swap(x1, x2);
	}
	
	if(y1 == y3)	// Single line triangles
	{
		xs = xe = x1;
		if(x2 < xs)			xs = x2;
		else if(x2 > xe)	xe = x2;
		if(x3 < xs)			xs = x3;
		else if(x3 > xe)	xe = x3;
		drawHLine(xs, y1, xe-xs);
		return;
	}
	
	// Upper part
	if (y2 == y3) ly = y2;
	else          ly = y2-1;
	
	for(y=y1; y<=ly; y++)
	{
		xs = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
		xe = x1 + (x3 - x1) * (y - y1) / (y3 - y1);
		drawHLine(xs, y, xe-xs);
	}
	
	// Lower part
	for(; y<=y3; y++)
	{
		xs = x2 + (x3 - x2) * (y - y2) / (y3 - y2);
		xe = x1 + (x3 - x1) * (y - y1) / (y3 - y1);
		drawHLine(xs, y, xe-xs);
	}
}

void TFT9341::drawArc(int x, int y, int r, int startAngle, int endAngle, int thickness)
{
	int rDelta = -(thickness/2);
	int px, py, cx, cy;

	startAngle -= 90;
	endAngle   -= 90;
	
	if (startAngle!=endAngle)
	{
		for (int i=0; i<thickness; i++)
		{
			px = x + cos((startAngle*3.14)/180) * (r+rDelta+i);
			py = y + sin((startAngle*3.14)/180) * (r+rDelta+i);
			for (int d=startAngle+1; d<endAngle+1; d++)
			{
				cx = x + cos((d*3.14)/180) * (r+rDelta+i);
				cy = y + sin((d*3.14)/180) * (r+rDelta+i);
				drawLine(px, py, cx, cy);
				px = cx;
				py = cy;
			}
		}
	}
	else
	{
		px = x + cos((startAngle*3.14)/180) * (r+rDelta);
		py = y + sin((startAngle*3.14)/180) * (r+rDelta);
		cx = x + cos((startAngle*3.14)/180) * (r-rDelta);
		cy = y + sin((startAngle*3.14)/180) * (r-rDelta);
		drawLine(px, py, cx, cy);
	}
}

void TFT9341::drawPie(int x, int y, int r, int startAngle, int endAngle)
{
	int px, py, cx, cy;

	startAngle -= 90;
	endAngle   -= 90;
	if (startAngle>endAngle)
		startAngle -= 360;
	
	px = x + cos((startAngle*3.14)/180) * r;
	py = y + sin((startAngle*3.14)/180) * r;
	drawLine(x, y, px, py);
	for (int d=startAngle+1; d<endAngle+1; d++)
	{
		cx = x + cos((d*3.14)/180) * r;
		cy = y + sin((d*3.14)/180) * r;
		drawLine(px, py, cx, cy);
		px = cx;
		py = cy;
	}
	drawLine(x, y, px, py);
}

void TFT9341::fillPie(int x, int y, int r, int startAngle, int endAngle)
{
    int px, py, cx, cy;
    
    startAngle -= 90;
    endAngle   -= 90;
    if (startAngle>endAngle)
        startAngle -= 360;
    
    px = x + cos((startAngle*3.14)/180) * r;
    py = y + sin((startAngle*3.14)/180) * r;
    drawLine(x, y, px, py);
    for (int d=startAngle+1; d<endAngle+1; d++)
    {
        cx = x + cos((d*3.14)/180) * r;
        cy = y + sin((d*3.14)/180) * r;
        drawLine(x, y, cx, cy);
    }
    drawLine(x, y, px, py);
}

void TFT9341::setTextColor(word fg, word bg) {
	setColor(fg);
	setBackColor(bg);
}
 void TFT9341::drawLine(int x1, int y1, int x2, int y2, word fg) {
	setColor(fg);
	drawLine(x1, y1, x2, y2);
}


// draw a proportional (or other ttf converted) font character on an angle
int TFT9341::rotatePropChar(byte c, int x, int y, int offset, int deg)
{
   propFont fontChar;
    
   if (!getCharPtr(c, fontChar))
   {
       return 0;
   }

   byte i,j,ch;
   word temp; 
   byte *tempPtr = fontChar.dataPtr;
	double radian = deg * 0.0175;  
   
   // fill background
   // VGA_TRANSPARENT?
   word fcolor = getColor();

	//cbi(P_CS, B_CS);
    
   if (fontChar.width != 0)
   {
      byte mask = 0x80;
      float cos_radian = cos(radian);
      float sin_radian = sin(radian);
      for (int j=0; j < fontChar.height; j++)
      {
         //ch=pgm_read_byte(tempPtr++);
         for (int i=0; i < fontChar.width; i++)
         {
            if (((i + (j*fontChar.width)) % 8) == 0)
            {
                mask = 0x80;
                ch = pgm_read_byte(tempPtr++);
            }
            
            int newX = x + ((offset + i) * cos_radian - (j+fontChar.adjYOffset)*sin_radian);
            int newY = y + ((j+fontChar.adjYOffset) * cos_radian + (offset + i) * sin_radian);
            if ((ch & mask) !=0)
            {
                setXY(newX,newY,newX,newY);
                setPixel(fcolor);
            } 
            else
            {
                if (!_transparent)
                {
                    setXY(newX,newY,newX,newY);
                    setPixel(getBackColor());
                }                
            }
            mask >>= 1;
         }
      }
   }

	//sbi(P_CS, B_CS);
	clrXY();
      
   return fontChar.xDelta;
}

// override UTFT::print to handle proportional and fixed-width fonts
void TFT9341::print(char *st, int x, int y, int deg)
{
	int stl, i;

	stl = strlen(st);

//	if (orient==PORTRAIT)
//	{
       if (x==RIGHT)
          x=disp_x_size-(stl*cfont.x_size);
       if (x==CENTER)
          x=(disp_x_size-(stl*cfont.x_size))/2;
/*	}
	else
	{
       if (x==RIGHT)
          x=(disp_y_size+1)-(stl*cfont.x_size);
       if (x==CENTER)
          x=((disp_y_size+1)-(stl*cfont.x_size))/2;
	}
*/
   int offset = 0;
	for (i=0; i < stl; i++)
   {
		if (deg==0)
      {
        // DLB Added this stuff...
        if (cfont.x_size == 0)
        {
            x += printProportionalChar(*st++, x, y)+1;
        }
        else
        {          
            printChar(*st++, x, y);
            x += cfont.x_size;
        }
      }
		else
      {
        // DLB Added this stuff...
        if (cfont.x_size == 0)
        {
            offset += rotatePropChar(*st++, x, y, offset, deg);
        }
        else
        {
            rotateChar(*st++, x, y, i, deg);
        }
      }
  }
}

// private method to return the Glyph data for an individual character in the ttf font
bool TFT9341::getCharPtr(byte c, propFont& fontChar)
{
    byte* tempPtr = cfont.font + 4; // point at data
    
    do
    {
        fontChar.charCode = pgm_read_byte(tempPtr++);
        fontChar.adjYOffset = pgm_read_byte(tempPtr++);
        fontChar.width = pgm_read_byte(tempPtr++);
        fontChar.height = pgm_read_byte(tempPtr++);
        fontChar.xOffset = pgm_read_byte(tempPtr++);
        fontChar.xOffset = fontChar.xOffset < 0x80 ? fontChar.xOffset : (0x100 - fontChar.xOffset);
        fontChar.xDelta = pgm_read_byte(tempPtr++);
        if (c != fontChar.charCode && fontChar.charCode != 0xFF)
        {
            if (fontChar.width != 0)
            {
                // packed bits
                tempPtr += (((fontChar.width * fontChar.height)-1) / 8) + 1;
            }
        }
    } while (c != fontChar.charCode && fontChar.charCode != 0xFF);
    
    fontChar.dataPtr = tempPtr;

    return (fontChar.charCode != 0xFF);
}


// print a ttf based character
int TFT9341::printProportionalChar(byte c, int x, int y)
{
	 byte i,j,ch;
	 word temp; 
    byte *tempPtr;

    propFont fontChar;    
    if (!getCharPtr(c, fontChar))
    {
        return 0;
    }
        
    // fill background
    // VGA_TRANSPARENT?
    word fcolor = getColor();
    if (!_transparent)
    {
        int fontHeight = getFontHeight();
        setColor(getBackColor());
        fillRect(x, y, x + fontChar.xDelta+1, y + fontHeight);
        setColor(fcolor);
    }
    
    tempPtr = fontChar.dataPtr;
    
    // draw Glyph
   	//*P_CS &= ~B_CS;
      
      if (fontChar.width != 0)
      {
          byte mask = 0x80;
          for (j=0; j < fontChar.height; j++)
          {
             //ch=pgm_read_byte(tempPtr++);
             for (i=0; i < fontChar.width; i++)
             {
                if (((i + (j*fontChar.width)) % 8) == 0)
                {
                    mask = 0x80;
                    ch = pgm_read_byte(tempPtr++);
                }
                
                if ((ch & mask) !=0)
                {
                    setXY(x+fontChar.xOffset+i, y+j+fontChar.adjYOffset,
                                x+fontChar.xOffset+i, y+j+fontChar.adjYOffset);
                    setPixel(fcolor);
                } 
                else
                {
                    //setPixel(bcolorr, bcolorg, bcolorb);
                }
                mask >>= 1;
             }
          }
      }
      
	 //*P_CS |= B_CS;
      
    return fontChar.xDelta;
}

// returns the string width in pixels. Useful for positions strings on the screen.
int TFT9341::getStringWidth(char* str)
{
    char* tempStrptr = str;
    
    // is it a fixed width font?
    if (cfont.x_size != 0)
    {
        return (strlen(str) * cfont.x_size);
    }
    else
    {
        // calculate the string width
        int strWidth = 0;
        while (*str != 0)
        {
            propFont fontChar;    
            bool found = getCharPtr(*str, fontChar);
            
            if (found && *str == fontChar.charCode)
            {
                strWidth += fontChar.xDelta + 1;
            }
            
            str++;            
        }

        return strWidth;
    }
}

int TFT9341::getFontHeight()
{
    return (cfont.y_size);
}

byte TFT9341::getOrientation()
{
    return orient;
}


void TFT9341::setupScrollArea(uint16_t TFA, uint16_t BFA) {
  TFT_CS_LOW;
  TFT_DC_LOW;
  SPI_TRANSFER(ILI9341_VSCRDEF);
  TFT_DC_HIGH;
  SPI_TRANSFER(TFA >> 8);
  SPI_TRANSFER(TFA);
  SPI_TRANSFER((320-TFA-BFA)>>8);
  SPI_TRANSFER(320-TFA-BFA);
  SPI_TRANSFER(BFA >> 8);
  SPI_TRANSFER(BFA);
  TFT_DC_HIGH;
  TFT_CS_HIGH;
    
}


void TFT9341::scrollAddress(uint16_t VSP) {
  TFT_CS_LOW;
  TFT_DC_LOW;
  SPI_TRANSFER(ILI9341_VSCRSADD); // Vertical scrolling start address
  TFT_DC_HIGH;
  SPI_TRANSFER(VSP>>8);
  SPI_TRANSFER(VSP);

  TFT_DC_HIGH;
  TFT_CS_HIGH;
}

/*
void TFT9341::readRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t *pcolors)
{
	uint16_t c = w * h;
	uint8_t r,g,b;
	
	setXY(x, y, x+w-1, y+h-1);
	TFT_CS_LOW;
	TFT_DC_LOW;
	SPI_TRANSFER(0x2E); // read from RAM
	TFT_DC_HIGH;
	SPI_TRANSFER(0x00); 
	
	
	while (c--) {
			r = SPI_TRANSFER(0x00);		// Read a RED byte of GRAM			
			g = SPI_TRANSFER(0x00);		// Read a GREEN byte of GRAM
			b = SPI_TRANSFER(0x00);		// Read a BLUE byte of GRAM			
			*pcolors++ = ((r&248)|g>>5);
			*pcolors++ = ((g&28)<<3|b>>3);
	}
	
	TFT_CS_HIGH;	
}

// Now lets see if we can writemultiple pixels
void TFT9341::writeRect(int16_t x, int16_t y, int16_t w, int16_t h, const uint8_t *pcolors)
{
    uint16_t c = w * h;
	setXY(x, y, x+w-1, y+h-1);
	TFT_CS_LOW;
	TFT_DC_HIGH;
	
    spi_write((void*)pcolors, c*2); 
		
	TFT_CS_HIGH;
}
*/

TFT9341 Tft=TFT9341();
