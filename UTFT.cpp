/*
  ILI9341 2.2 TFT SPI library
  based on UTFT.cpp - Arduino/chipKit library support for Color TFT LCD Boards
  Copyright (C)2010-2013 Henning Karlsen. All right reserved
 
  Compatible with other UTFT libraries.
 
  Original library you can find at http://electronics.henningkarlsen.com/
 
 Pinout:
 
 (Arduino UNO : TFT)
    D4  : RESET
    D5  : CS
    D6  : D/C
    D7  : LED
 
    D11 : MOSI
    D12 : MISO
    D13 : SCK
 
  This library is free software; you can redistribute it and/or
  modify it under the terms of the CC BY-NC-SA 3.0 license.
  Please see the included documents for further information.
*/



#include "UTFT.h"
#include <pins_arduino.h>
#include <SPI.h>
#include <digitalWriteFast.h>

#define TFT_BL_OFF digitalWriteFast(LED,LOW) 
#define TFT_BL_ON  digitalWriteFast(LED,HIGH)

#define TFT_RST_OFF digitalWriteFast(RESET, HIGH) 
#define TFT_RST_ON  digitalWriteFast(RESET,LOW) 


#define TFT_CS_LOW digitalWriteFast(CS,LOW)
#define TFT_CS_HIGH digitalWriteFast(CS,HIGH) 

#define TFT_DC_LOW  digitalWriteFast(DC,LOW) 
#define TFT_DC_HIGH digitalWriteFast(DC,HIGH) 

void UTFT::sendCMD(INT8U index)
{
    TFT_DC_LOW;
    TFT_CS_LOW;
    SPI.transfer(index);
    TFT_CS_HIGH;
}

void UTFT::WRITE_DATA(INT8U data)
{
    TFT_DC_HIGH;
    TFT_CS_LOW;
    SPI.transfer(data);
    TFT_CS_HIGH;
}

void UTFT::sendData(INT16U data)
{
    INT8U data1 = data>>8;
    INT8U data2 = data&0xff;
    TFT_DC_HIGH;
    TFT_CS_LOW;
    SPI.transfer(data1);
    SPI.transfer(data2);
    TFT_CS_HIGH;
}

void UTFT::WRITE_Package(INT16U *data, INT8U howmany)
{
    INT16U    data1 = 0;
    INT8U   data2 = 0;
    
    TFT_DC_HIGH;
    TFT_CS_LOW;
    INT8U count=0;
    for(count=0;count<howmany;count++)
    {
        data1 = data[count]>>8;
        data2 = data[count]&0xff;
        SPI.transfer(data1);
        SPI.transfer(data2);
    }
    TFT_CS_HIGH;
}

void UTFT::LCD_Write_COM(char VL)
{
    sendCMD(VL);
}

void UTFT::LCD_Write_DATA(char VH,char VL)
{
    WRITE_DATA(VH);
    WRITE_DATA(VL);
}

void UTFT::LCD_Write_DATA(char VL)
{
    WRITE_DATA(VL);
}


INT8U UTFT::Read_Register(INT8U Addr, INT8U xParameter)
{
    INT8U data=0;
    sendCMD(0xd9);                                                      /* ext command                  */
    WRITE_DATA(0x10+xParameter);                                        /* 0x11 is the first Parameter  */
    TFT_DC_LOW;
    TFT_CS_LOW;
    SPI.transfer(Addr);
    TFT_DC_HIGH;
    data = SPI.transfer(0);
    TFT_CS_HIGH;
    return data;
}

INT8U UTFT::readID(void)
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
    /*
    if(!ToF)                                                            // data!=ID
    {
        
        Serial.print("Read TFT ID failed, ID should be 0x09341, but read ID = 0x");
        for(i=0;i<3;i++)
        {
            Serial.print(data[i],HEX);
        }
        Serial.println();
        
        
    }
    */
    delay(10);
    return ToF;
}

void UTFT::InitLCD(byte orientation)
{

	pinModeFast(LED,OUTPUT);
	pinModeFast(RESET,OUTPUT);
	pinModeFast(CS,OUTPUT);
	pinModeFast(DC,OUTPUT);
    
    disp_x_size=239;
    disp_y_size=319;
    
    TFT_BL_ON;
	orient=orientation;
    SPI.begin();
#ifdef FASTSPI
    SPI.setClockDivider( SPI_CLOCK_DIV2 );
#endif
    TFT_CS_HIGH;
    TFT_DC_HIGH;
    INT8U i=0, TFTDriver=0;
    
	TFT_RST_ON;
	delay(10);
	TFT_RST_OFF;
    delay(10);
    /*
    for(i=0;i<3;i++)
    {
        TFTDriver = readID();
    }
    */
	sendCMD(0xCB);
	WRITE_DATA(0x39);
	WRITE_DATA(0x2C);
	WRITE_DATA(0x00);
	WRITE_DATA(0x34);
	WRITE_DATA(0x02);
    
	sendCMD(0xCF);
	WRITE_DATA(0x00);
	WRITE_DATA(0XC1);
	WRITE_DATA(0X30);
    
	sendCMD(0xE8);
	WRITE_DATA(0x85);
	WRITE_DATA(0x00);
	WRITE_DATA(0x78);
    
	sendCMD(0xEA);
	WRITE_DATA(0x00);
	WRITE_DATA(0x00);
    
	sendCMD(0xED);
	WRITE_DATA(0x64);
	WRITE_DATA(0x03);
	WRITE_DATA(0X12);
	WRITE_DATA(0X81);
    
	sendCMD(0xF7);
	WRITE_DATA(0x20);
    
	sendCMD(0xC0);    	//Power control
	WRITE_DATA(0x23);   	//VRH[5:0]
    
	sendCMD(0xC1);    	//Power control
	WRITE_DATA(0x10);   	//SAP[2:0];BT[3:0]
    
	sendCMD(0xC5);    	//VCM control
	WRITE_DATA(0x3e);   	//Contrast
	WRITE_DATA(0x28);
    
	sendCMD(0xC7);    	//VCM control2
	WRITE_DATA(0x86);  	 //--
    
	sendCMD(0x36);    	// Memory Access Control
	WRITE_DATA(0x48);  	//C8	   //48 68绔栧睆//28 E8 妯睆
    
	sendCMD(0x3A);
	WRITE_DATA(0x55);
    
	sendCMD(0xB1);
	WRITE_DATA(0x00);
	WRITE_DATA(0x18);
    
	sendCMD(0xB6);    	// Display Function Control
	WRITE_DATA(0x08);
	WRITE_DATA(0x82);
	WRITE_DATA(0x27);
    
	sendCMD(0xF2);    	// 3Gamma Function Disable
	WRITE_DATA(0x00);
    
	sendCMD(0x26);    	//Gamma curve selected
	WRITE_DATA(0x01);
    
	sendCMD(0xE0);    	//Set Gamma
	WRITE_DATA(0x0F);
	WRITE_DATA(0x31);
	WRITE_DATA(0x2B);
	WRITE_DATA(0x0C);
	WRITE_DATA(0x0E);
	WRITE_DATA(0x08);
	WRITE_DATA(0x4E);
	WRITE_DATA(0xF1);
	WRITE_DATA(0x37);
	WRITE_DATA(0x07);
	WRITE_DATA(0x10);
	WRITE_DATA(0x03);
	WRITE_DATA(0x0E);
	WRITE_DATA(0x09);
	WRITE_DATA(0x00);
    
	sendCMD(0XE1);    	//Set Gamma
	WRITE_DATA(0x00);
	WRITE_DATA(0x0E);
	WRITE_DATA(0x14);
	WRITE_DATA(0x03);
	WRITE_DATA(0x11);
	WRITE_DATA(0x07);
	WRITE_DATA(0x31);
	WRITE_DATA(0xC1);
	WRITE_DATA(0x48);
	WRITE_DATA(0x08);
	WRITE_DATA(0x0F);
	WRITE_DATA(0x0C);
	WRITE_DATA(0x31);
	WRITE_DATA(0x36);
	WRITE_DATA(0x0F);
    
	sendCMD(0x11);    	//Exit Sleep
	delay(120); 
    
	sendCMD(0x29);    //Display on 
	sendCMD(0x2c);
    
	cfont.font=0;
	_transparent = false;
}

void UTFT::setXY(word x1, word y1, word x2, word y2)
{
	int tmp;

	if (orient==LANDSCAPE)
	{
		swap(word, x1, y1);
		swap(word, x2, y2)
		y1=disp_y_size-y1;
		y2=disp_y_size-y2;
		swap(word, y1, y2)
	}

    TFT_DC_LOW;
    TFT_CS_LOW;
    SPI.transfer(0x2a);
    TFT_DC_HIGH;
	SPI.transfer(x1>>8);
	SPI.transfer(x1);
	SPI.transfer(x2>>8);
	SPI.transfer(x2);
    TFT_DC_LOW;
    SPI.transfer(0x2b);
    TFT_DC_HIGH;
	SPI.transfer(y1>>8);
	SPI.transfer(y1);
	SPI.transfer(y2>>8);
	SPI.transfer(y2);
	TFT_DC_LOW;
    SPI.transfer(0x2c);
    TFT_CS_HIGH;
}

void UTFT::clrXY()
{
	if (orient==PORTRAIT)
		setXY(0,0,disp_x_size,disp_y_size);
	else
		setXY(0,0,disp_y_size,disp_x_size);
}

void UTFT::drawRect(int x1, int y1, int x2, int y2)
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


void UTFT::fillRect(int x1, int y1, int x2, int y2)
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

		if (orient==PORTRAIT)
		{
			for (int i=0; i<((y2-y1)/2)+1; i++)
			{
				drawHLine(x1, y1+i, x2-x1);
				drawHLine(x1, y2-i, x2-x1);
			}
		}
		else
		{
			for (int i=0; i<((x2-x1)/2)+1; i++)
			{
				drawVLine(x1+i, y1, y2-y1);
				drawVLine(x2-i, y1, y2-y1);
			}
		}
	}



void UTFT::drawRoundRect(int x1, int y1, int x2, int y2)
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

void UTFT::fillRoundRect(int x1, int y1, int x2, int y2)
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

void UTFT::drawCircle(int x, int y, int radius)
{
	int f = 1 - radius;
	int ddF_x = 1;
	int ddF_y = -2 * radius;
	int x1 = 0;
	int y1 = radius;
 
	//cbi(P_CS, B_CS);
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
	//sbi(P_CS, B_CS);
	clrXY();
}

void UTFT::fillCircle(int x, int y, int radius)
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


void UTFT::clrScr()
{
	long i;
    clrXY();
    TFT_DC_HIGH;
    TFT_CS_LOW;
    for (int i=0; i<((disp_x_size+1)*(disp_y_size+1)); i++)
    {
        // black
	    SPI.transfer(0);
    	SPI.transfer(0);
    }
     TFT_CS_HIGH;
}

void UTFT::fillScr(byte r, byte g, byte b)
{
	word color = ((r&248)<<8 | (g&252)<<3 | (b&248)>>3);
	fillScr(color);
}

void UTFT::fillScr(word color)
{
	long i;
	char ch, cl;
	
	ch=byte(color>>8);
	cl=byte(color & 0xFF);
	
    clrXY();
    TFT_DC_HIGH;
    TFT_CS_LOW;
    for (int i=0; i<((disp_x_size+1)*(disp_y_size+1)); i++)
    {
	    SPI.transfer(ch);
    	SPI.transfer(cl);
    }
    TFT_CS_HIGH;
}

void UTFT::setColor(byte r, byte g, byte b)
{
	fch=((r&248)|g>>5);
	fcl=((g&28)<<3|b>>3);
}

void UTFT::setColor(word color)
{
	fch=byte(color>>8);
	fcl=byte(color & 0xFF);
}

word UTFT::getColor()
{
	return (fch<<8) | fcl;
}

void UTFT::setBackColor(byte r, byte g, byte b)
{
	bch=((r&248)|g>>5);
	bcl=((g&28)<<3|b>>3);
	_transparent=false;
}

void UTFT::setBackColor(uint32_t color)
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

word UTFT::getBackColor()
{
	return (bch<<8) | bcl;
}

void UTFT::setPixel(word color)
{
	LCD_Write_DATA((color>>8),(color&0xFF));	// rrrrrggggggbbbbb
}

void UTFT::drawPixel(int x, int y)
{

	setXY(x, y, x, y);
	setPixel((fch<<8)|fcl);
	clrXY();
}

void UTFT::drawLine(int x1, int y1, int x2, int y2)
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
				LCD_Write_DATA (fch, fcl);
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

void UTFT::drawHLine(int x, int y, int l)
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
			SPI.transfer(fch);
			SPI.transfer(fcl);
		}
	TFT_CS_HIGH;

	clrXY();
}

void UTFT::drawVLine(int x, int y, int l)
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
			SPI.transfer(fch);
			SPI.transfer(fcl);
		}
    TFT_CS_HIGH;
	clrXY();
}

void UTFT::printChar(byte c, int x, int y)
{
	byte i,ch;
	word j;
	word temp; 


  
	if (!_transparent)
	{
		if (orient==PORTRAIT)
		{
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
		}
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
		}
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

void UTFT::rotateChar(byte c, int x, int y, int pos, int deg)
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

void UTFT::print(char *st, int x, int y, int deg)
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

void UTFT::printC(String st, int x, int y, uint32_t color)
{
    char buf[st.length()+1];
    setColor(color);
    st.toCharArray(buf, st.length()+1);
    print(buf, x, y, 0);
}

void UTFT::print(String st, int x, int y, int deg)
{
	char buf[st.length()+1];
    st.toCharArray(buf, st.length()+1);
	print(buf, x, y, deg);
}

void UTFT::printNumI(long num, int x, int y, int length, char filler)
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

void UTFT::printNumF(double num, byte dec, int x, int y, char divider, int length, char filler)
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

void UTFT::setFont(uint8_t* font)
{
	cfont.font=font;
	cfont.x_size=fontbyte(0);
	cfont.y_size=fontbyte(1);
	cfont.offset=fontbyte(2);
	cfont.numchars=fontbyte(3);
}

uint8_t* UTFT::getFont()
{
	return cfont.font;
}

uint8_t UTFT::getFontXsize()
{
	return cfont.x_size;
}

uint8_t UTFT::getFontYsize()
{
	return cfont.y_size;
}

void UTFT::drawBitmap(int x, int y, int sx, int sy, bitmapdatatype data, int scale)
{
	unsigned int col;
	int tx, ty, tc, tsx, tsy;
	byte r, g, b;

	if (scale==1)
	{
		if (orient==PORTRAIT)
		{

			setXY(x, y, x+sx-1, y+sy-1);
			for (tc=0; tc<(sx*sy); tc++)
			{
				col=pgm_read_word(&data[tc]);
				LCD_Write_DATA(col>>8,col & 0xff);
			}
					}
		else
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

		}
	}
	else
	{
		if (orient==PORTRAIT)
		{

			for (ty=0; ty<sy; ty++)
			{
				setXY(x, y+(ty*scale), x+((sx*scale)-1), y+(ty*scale)+scale);
				for (tsy=0; tsy<scale; tsy++)
					for (tx=0; tx<sx; tx++)
					{
						col=pgm_read_word(&data[(ty*sx)+tx]);
						for (tsx=0; tsx<scale; tsx++)
							LCD_Write_DATA(col>>8,col & 0xff);
					}
			}

		}
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

		}
	}
	clrXY();
}

void UTFT::drawBitmap(int x, int y, int sx, int sy, bitmapdatatype data, int deg, int rox, int roy)
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

void UTFT::lcdOff()
{
    TFT_BL_OFF;
}

void UTFT::lcdOn()
{
    TFT_BL_ON;
}

/*
void UTFT::setContrast(char c)
{

}
*/

int UTFT::getDisplayXSize()
{
	if (orient==PORTRAIT)
		return disp_x_size+1;
	else
		return disp_y_size+1;
}

int UTFT::getDisplayYSize()
{
	if (orient==PORTRAIT)
		return disp_y_size+1;
	else
		return disp_x_size+1;
}

#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04

void UTFT::setRotation(uint8_t m) {
    sendCMD(0x36);
    rotation = m % 4; // can't be higher than 3
    switch (rotation) {
        case 0:
            WRITE_DATA(MADCTL_MX | MADCTL_BGR);
            orient=LANDSCAPE;
            break;
        case 1:
            WRITE_DATA(MADCTL_MV | MADCTL_BGR);
            orient=PORTRAIT;
            break;
        case 2:
            WRITE_DATA(MADCTL_MY | MADCTL_BGR);
            orient=LANDSCAPE;
            break;
        case 3:
            WRITE_DATA(MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
            orient=PORTRAIT;
            break;
    }
}

UTFT Tft=UTFT();
