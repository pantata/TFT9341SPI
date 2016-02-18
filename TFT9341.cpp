/*
  ILI9341 2.0 TFT SPI  DMA library
  Compatible with  UTFT libraries.
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the CC BY-NC-SA 3.0 license.
  Please see the included documents for further information.
*/

#include "Arduino.h"
#include "TFT9341.h"
#include <pins_arduino.h>
#include <SPI.h>

#if defined(__SAMD21G18A__)
	/*
	 * DMA
	 *  
	 */

	typedef struct {
		uint16_t btctrl;
		uint16_t btcnt;
		uint32_t srcaddr;
		uint32_t dstaddr;
		uint32_t descaddr;
	} dmacdescriptor ;

	volatile dmacdescriptor wrb[12] __attribute__ ((aligned (16)));
	dmacdescriptor descriptor_section[12] __attribute__ ((aligned (16)));
	dmacdescriptor descriptor __attribute__ ((aligned (16)));

	static uint32_t chnltx = 0, chnlrx = 1; // DMA channels
	enum XfrType { DoTX, DoRX, DoTXRX};
	static XfrType xtype;
	static uint8_t rxsink[1], txsrc[1] = {0xff};
	volatile uint32_t dmadone;

	void DMAC_Handler() {
		// interrupts DMAC_CHINTENCLR_TERR DMAC_CHINTENCLR_TCMPL DMAC_CHINTENCLR_SUSP
		uint8_t active_channel;

		// disable irqs ?
		__disable_irq();
		active_channel =  DMAC->INTPEND.reg & DMAC_INTPEND_ID_Msk; // get channel number
		DMAC->CHID.reg = DMAC_CHID_ID(active_channel);
		dmadone = DMAC->CHINTFLAG.reg;
		DMAC->CHINTFLAG.reg = DMAC_CHINTENCLR_TCMPL; // clear
		DMAC->CHINTFLAG.reg = DMAC_CHINTENCLR_TERR;
		DMAC->CHINTFLAG.reg = DMAC_CHINTENCLR_SUSP;
		__enable_irq();
	}

	void dma_init() {
		// probably on by default
		PM->AHBMASK.reg |= PM_AHBMASK_DMAC ;
		PM->APBBMASK.reg |= PM_APBBMASK_DMAC ;
		NVIC_EnableIRQ( DMAC_IRQn ) ;

		DMAC->BASEADDR.reg = (uint32_t)descriptor_section;
		DMAC->WRBADDR.reg = (uint32_t)wrb;
		DMAC->CTRL.reg = DMAC_CTRL_DMAENABLE | DMAC_CTRL_LVLEN(0xf);
	}


	Sercom *sercom = (Sercom   *)SERCOM4;  // SPI SERCOM

	void spi_xfr(void *txdata, void *rxdata,  size_t n) {
		uint32_t temp_CHCTRLB_reg;

		// set up transmit channel  
		DMAC->CHID.reg = DMAC_CHID_ID(chnltx); 
		DMAC->CHCTRLA.reg &= ~DMAC_CHCTRLA_ENABLE;
		DMAC->CHCTRLA.reg = DMAC_CHCTRLA_SWRST;
		DMAC->SWTRIGCTRL.reg &= (uint32_t)(~(1 << chnltx));
		temp_CHCTRLB_reg = DMAC_CHCTRLB_LVL(0) | 
		  DMAC_CHCTRLB_TRIGSRC(SERCOM4_DMAC_ID_TX) | DMAC_CHCTRLB_TRIGACT_BEAT;
		DMAC->CHCTRLB.reg = temp_CHCTRLB_reg;
		DMAC->CHINTENSET.reg = DMAC_CHINTENSET_MASK ; // enable all 3 interrupts
		descriptor.descaddr = 0;
		descriptor.dstaddr = (uint32_t) &sercom->SPI.DATA.reg;
		descriptor.btcnt =  n;
		descriptor.srcaddr = (uint32_t)txdata;
		descriptor.btctrl =  DMAC_BTCTRL_VALID;
		if (xtype != DoRX) {
			descriptor.srcaddr += n;
			descriptor.btctrl |= DMAC_BTCTRL_SRCINC;
		}
		memcpy(&descriptor_section[chnltx],&descriptor, sizeof(dmacdescriptor));

		// rx channel    enable interrupts
		DMAC->CHID.reg = DMAC_CHID_ID(chnlrx); 
		DMAC->CHCTRLA.reg &= ~DMAC_CHCTRLA_ENABLE;
		DMAC->CHCTRLA.reg = DMAC_CHCTRLA_SWRST;
		DMAC->SWTRIGCTRL.reg &= (uint32_t)(~(1 << chnlrx));
		temp_CHCTRLB_reg = DMAC_CHCTRLB_LVL(0) | 
		  DMAC_CHCTRLB_TRIGSRC(SERCOM4_DMAC_ID_RX) | DMAC_CHCTRLB_TRIGACT_BEAT;
		DMAC->CHCTRLB.reg = temp_CHCTRLB_reg;
		DMAC->CHINTENSET.reg = DMAC_CHINTENSET_MASK ; // enable all 3 interrupts
		dmadone = 0;
		descriptor.descaddr = 0;
		descriptor.srcaddr = (uint32_t) &sercom->SPI.DATA.reg;
		descriptor.btcnt =  n;
		descriptor.dstaddr = (uint32_t)rxdata;
		descriptor.btctrl =  DMAC_BTCTRL_VALID;
		if (xtype != DoTX) {
			descriptor.dstaddr += n;
			descriptor.btctrl |= DMAC_BTCTRL_DSTINC;
		}
		memcpy(&descriptor_section[chnlrx],&descriptor, sizeof(dmacdescriptor));

		// start both channels  ? order matter ?
		DMAC->CHID.reg = DMAC_CHID_ID(chnltx);
		DMAC->CHCTRLA.reg |= DMAC_CHCTRLA_ENABLE;
		DMAC->CHID.reg = DMAC_CHID_ID(chnlrx);
		DMAC->CHCTRLA.reg |= DMAC_CHCTRLA_ENABLE;

		while(!dmadone);  // await DMA done isr

		DMAC->CHID.reg = DMAC_CHID_ID(chnltx);   //disable DMA to allow lib SPI 
		DMAC->CHCTRLA.reg &= ~DMAC_CHCTRLA_ENABLE;
		DMAC->CHID.reg = DMAC_CHID_ID(chnlrx); 
		DMAC->CHCTRLA.reg &= ~DMAC_CHCTRLA_ENABLE;
	}

	void spi_write(void *data,  size_t n) {
		xtype = DoTX;
		spi_xfr(data,rxsink,n);
	}
	void spi_read(void *data,  size_t n) {
		xtype = DoRX;
		spi_xfr(txsrc,data,n);
	}

	void spi_transfer(void *txdata, void *rxdata,  size_t n) {
		xtype = DoTXRX;
		spi_xfr(txdata,rxdata,n);
	}
	
#elif defined(__AVR__)

	void spi_write(void *data,  size_t n) {	
		byte c;
		uint16_t i = 0;	
		while(n--)			
			SPI.transfer(*((byte*)data+i));
			i++;
	}	
	
#endif

/*
 * Pin setup
 *
 */
#if defined(__SAMD21G18A__)
	
	#define PINMODE(x,y) pinMode(x,y) 
	#define SPI_TRANSFER(x) SPI.transfer(x)

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

#elif defined(__AVR__)
	
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
	
	
#endif

  
static const uint8_t PROGMEM init_cmd[] = {
	4, 0xEF, 0x03, 0x80, 0x02,
	4, 0xCF, 0x00, 0XC1, 0X30,
	5, 0xED, 0x64, 0x03, 0X12, 0X81,
	4, 0xE8, 0x85, 0x00, 0x78,
	6, 0xCB, 0x39, 0x2C, 0x00, 0x34, 0x02,
	2, 0xF7, 0x20,
	3, 0xEA, 0x00, 0x00,
	2, PWCTR1, 0x23, // Power control
	2, PWCTR2, 0x10, // Power control
	3, VMCTR1, 0x3e, 0x28, // VCM control
	2, VMCTR2, 0x86, // VCM control2
	2, MADCTL, 0x48, // Memory Access Control
	2, PIXFMT, 0x55,
	3, FRMCTR1, 0x00, 0x18,
	4, DFUNCTR, 0x08, 0x82, 0x27, // Display Function Control
	2, 0xF2, 0x00, // Gamma Function Disable
	2, GAMMASET, 0x01, // Gamma curve selected
	16, GMCTRP1, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08,
	0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00, // Set Gamma
	16, GMCTRN1, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07,
	0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F, // Set Gamma
	0
};
 
 
/*
 * DC for command, writes 1 byte
 * 
 */ 
inline __attribute__((always_inline)) void wr_comm_first(uint8_t c) {
		char ch = c;
		TFT_DC_LOW;
		spi_write(&ch,1);
} 
 
/*
 * DC for command, writes 1 byte
 * set DC for data
 */ 
inline __attribute__((always_inline)) void wr_comm_last(uint8_t c) {
		char ch = c;  
		TFT_DC_LOW;
		spi_write(&ch,1);
		TFT_DC_HIGH;
}

/*
 * Set DC to data, writes 1 byte
 * not disable CS
 */
inline __attribute__((always_inline)) void write8_cont(uint8_t c) {
		char ch = c;  
		spi_write(&ch,1);
}
 
TFT9341::TFT9341() {}

void TFT9341::InitLCD(uint8_t orientation) {
		
	PINMODE(LED,OUTPUT);
	PINMODE(RESET,OUTPUT);
	PINMODE(CS,OUTPUT);
	PINMODE(DC,OUTPUT);
    
    TFT_BL_ON;
	SPI.begin();
	SPI.setBitOrder(MSBFIRST);
	SPI.setDataMode(SPI_MODE0);	
	SPI.setClockDivider(2);		
	dma_init();
	
	//reset
	TFT_CS_HIGH;
    TFT_DC_HIGH;
    TFT_RST_OFF;
    delay(5);
	TFT_RST_ON;
	delay(20);
	TFT_RST_OFF;
    delay(100);
    
    //init commands & values
    const uint8_t *adr = init_cmd;
	TFT_CS_LOW;
	while (1) {
		uint8_t count = *(adr++);		
		if (count-- == 0) break;
		wr_comm_last(*(adr++));
		while (count-- > 0) {
			write8_cont(*(adr++));
		}
	}
	wr_comm_first(0x11);
	delay(120);
	write8_cont(0x29);
	write8_cont(0x2c);	
	TFT_CS_HIGH;		
    
	cfont.font=0;
	_transparent = false;
	_fgc = VGA_WHITE;
    _bgc = VGA_BLACK;
	setRotation(orientation);	
}

void TFT9341::setRotation(uint8_t m) {
    uint8_t o;
    rotation = m % 4; // can't be higher than 3
    switch (rotation) {
        case 0:
            o = MADCTL_MX | MADCTL_BGR;
            orient=PORTRAIT;
            disp_x_size=239;
		    disp_y_size=319;            
            break;
        case 1:
             o = MADCTL_MV | MADCTL_BGR;
            orient=LANDSCAPE;
            disp_y_size=239;
		    disp_x_size=319;            
            break;
        case 2:
             o = MADCTL_MY | MADCTL_BGR;
            orient=PORTRAIT;
            disp_x_size=239;
		    disp_y_size=319;            
            break;
        case 3:
             o = MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR;
            orient=LANDSCAPE;
            disp_y_size=239;
		    disp_x_size=319;            
            break;
    }
    TFT_CS_LOW;
    wr_comm_last(MADCTL);
    write8_cont(o);
    TFT_CS_HIGH;
}

int  TFT9341::getDisplayXSize() {
	return disp_x_size;
}
int	 TFT9341::getDisplayYSize() {
	return disp_y_size;
}
		
void TFT9341::setXY(word x1, word y1, word x2, word y2) {
    uint8_t x[] = {x1>>8,x1,x2>>8,x2};
    uint8_t y[] = {y1>>8,y1,y2>>8,y2};
    
    wr_comm_last(CASET);
    spi_write(x, sizeof(x));
	wr_comm_last(PASET);
	spi_write(y, sizeof(y));
}

void TFT9341::clrScr() {
	const uint32_t lines = (uint32_t)76800 / (uint32_t)320;
	memset(scanline,0,sizeof(scanline));
			
    TFT_CS_LOW;
	setXY(0,0,disp_x_size,disp_y_size);	
    wr_comm_last(RAMWR);
    for (uint32_t i = 0; i < lines; i++) {
		spi_write(scanline,sizeof(scanline));
	}
    TFT_CS_HIGH;
}

void TFT9341::drawHLine(int x, int y, int l) {

	if (l < 0) {
		l = -l;
		x -= l;
	}
	
	for (uint16_t i = 0; i < l; i++) {
		scanline[i] = (uint8_t(_fgc & 0xFF) <<8 )|( uint8_t(_fgc>>8));
	}

    TFT_CS_LOW;
	setXY(x, y, x+l, y);
	wr_comm_last(RAMWR);
	spi_write(scanline,l*2);
    TFT_CS_HIGH;
//	clrXY();
}

void TFT9341::drawVLine(int x, int y, int l) {
	if (l<0) {
		l = -l;
		y -= l;
	}

	for (uint16_t i = 0; i < l; i++) {
		scanline[i] = (uint8_t(_fgc & 0xFF) <<8 )|( uint8_t(_fgc>>8));
	}
	TFT_CS_LOW;
	setXY(x, y, x, y+l);
	wr_comm_last(RAMWR);
	spi_write(scanline,l*2);
    TFT_CS_HIGH;    
//	clrXY();	
}

void TFT9341::drawRect(int x1, int y1, int x2, int y2)
{
	if (x1>x2)	{
		swap(int, x1, x2);
	}
	if (y1>y2) 	{
		swap(int, y1, y2);
	}
	
	drawHLine(x1, y1, x2-x1);
	drawHLine(x1, y2, x2-x1);
	drawVLine(x1, y1, y2-y1);
	drawVLine(x2, y1, y2-y1);	
}

void TFT9341::drawPixel(int x, int y)
{	
	TFT_CS_LOW;
	setXY(x, y, x, y);
	wr_comm_last(RAMWR);
	uint16_t color = (uint8_t(_fgc & 0xFF) <<8 )|( uint8_t(_fgc>>8));
	spi_write(&color,2);
	TFT_CS_HIGH;
}

void TFT9341::drawLine(int x1, int y1, int x2, int y2)
{
	uint16_t color = (uint8_t(_fgc & 0xFF) <<8 )|( uint8_t(_fgc>>8));
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
		
		TFT_CS_LOW;
		if (dx < dy)
		{
			int t = - (dy >> 1);
			while (true)
			{
				setXY (col, row, col, row);
				wr_comm_last(RAMWR);
				spi_write(&color,2);
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
				wr_comm_last(RAMWR);
				spi_write(&color,2);
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
		TFT_CS_HIGH;	
	}

}

void TFT9341::setColor(uint8_t r, uint8_t g, uint8_t b) {
	fch=((r&248)|g>>5);
	fcl=((g&28)<<3|b>>3);
	_fgc = (fch<<8) | fcl;
}
void TFT9341::setColor(word color) {
	fch=uint8_t(color>>8);
	fcl=uint8_t(color & 0xFF);
	_fgc = color;
}

void TFT9341::setBackColor(uint8_t r, uint8_t g, uint8_t b) {
	bch=((r&248)|g>>5);
	bcl=((g&28)<<3|b>>3);
	_transparent=false;
	_bgc = (bch<<8) | bcl;
}

void TFT9341::setBackColor(uint32_t color) {
	bch=uint8_t(color>>8);
	bcl=uint8_t(color & 0xFF);
	_bgc = color;
}

uint16_t TFT9341::getColor() {
	return _fgc;
}
uint16_t TFT9341::getBackColor() {
	return _bgc;
}

void TFT9341::fillScr(uint8_t r, uint8_t g, uint8_t b) {
	uint16_t color = ((r&248)<<8 | (g&252)<<3 | (b&248)>>3);
	fillScr(color);
}

void TFT9341::fillScr(uint16_t color) {

	const uint32_t lines = (uint32_t)76800 / (uint32_t)320;
	for (uint16_t i = 0; i < 320; i++) {
		scanline[i] = (uint8_t(color & 0xFF) <<8 )|( uint8_t(color>>8));
	}
			
    TFT_CS_LOW;
	setXY(0,0,disp_x_size,disp_y_size);	
    wr_comm_last(RAMWR);
    for (uint32_t i = 0; i < lines; i++) {
		spi_write(scanline,sizeof(scanline));
	}
    TFT_CS_HIGH;
}

void TFT9341::fillRect(int x1, int y1, int x2, int y2)
{

	if (x1>x2)
	{
		swap(int, x1, x2);
	}
	if (y1>y2)
	{
		swap(int, y1, y2);
	}
	
	uint16_t length = x2-x1;   //317
	uint16_t height = y2-y1;    //209	 
	uint32_t pxcnt = length * height;  //66253
	
	uint16_t cnt = (pxcnt<= 320)?pxcnt:320;
	

	for (uint16_t i = 0; i < cnt ; i++) {
			scanline[i] = (uint8_t(_fgc & 0xFF) <<8 )|( uint8_t(_fgc>>8));
	}	

	TFT_CS_LOW;
	setXY(x1,y1,x2,y2);	
	wr_comm_last(RAMWR);
	if (pxcnt <= 320) {
		spi_write(scanline, pxcnt*2);
	} else {
		uint16_t lines = pxcnt / 320;
		for (uint32_t i = 0; i < lines; i++) {  //207
			spi_write(scanline,640);
		}
		uint16_t rpx = pxcnt % 320;
		if (rpx > 0) spi_write(scanline,rpx*2);
	}
	TFT_CS_HIGH;
}

void TFT9341::drawRoundRect(int x1, int y1, int x2, int y2)
{
    
    if (x1>x2) {
        swap(int, x1, x2);
    }
    if (y1>y2) {
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
	if (x1>x2) {
		swap(int, x1, x2);
	}
	if (y1>y2) {
		swap(int, y1, y2);
	}

	if ((x2-x1)>4 && (y2-y1)>4) {
		for (int i=0; i<((y2-y1)/2)+1; i++) {
			switch(i) {
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
	uint16_t color = (uint8_t(_fgc & 0xFF) <<8 )|( uint8_t(_fgc>>8));

	int f = 1 - radius;
	int ddF_x = 1;
	int ddF_y = -2 * radius;
	int x1 = 0;
	int y1 = radius;

 	TFT_CS_LOW;
	setXY(x, y + radius, x, y + radius);
	wr_comm_last(RAMWR);
	spi_write(&color,2);
	setXY(x, y - radius, x, y - radius);
	wr_comm_last(RAMWR);
	spi_write(&color,2);
	setXY(x + radius, y, x + radius, y);
	wr_comm_last(RAMWR);
	spi_write(&color,2);
	setXY(x - radius, y, x - radius, y);
	wr_comm_last(RAMWR);spi_write(&color,2);

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
		wr_comm_last(RAMWR);spi_write(&color,2);
		setXY(x - x1, y + y1, x - x1, y + y1);
		wr_comm_last(RAMWR);spi_write(&color,2);
		setXY(x + x1, y - y1, x + x1, y - y1);
		wr_comm_last(RAMWR);spi_write(&color,2);
		setXY(x - x1, y - y1, x - x1, y - y1);
		wr_comm_last(RAMWR);spi_write(&color,2);
		setXY(x + y1, y + x1, x + y1, y + x1);
		wr_comm_last(RAMWR);spi_write(&color,2);
		setXY(x - y1, y + x1, x - y1, y + x1);
		wr_comm_last(RAMWR);spi_write(&color,2);
		setXY(x + y1, y - x1, x + y1, y - x1);
		wr_comm_last(RAMWR);spi_write(&color,2);
		setXY(x - y1, y - x1, x - y1, y - x1);
		wr_comm_last(RAMWR);spi_write(&color,2);
	}
	TFT_CS_HIGH;
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

void TFT9341::lcdOff()
{
    TFT_BL_OFF;
}

void TFT9341::lcdOn()
{
    TFT_BL_ON;
}

void TFT9341::setPixel(uint16_t color)
{
	uint16_t _c = (uint8_t(color & 0xFF) <<8 )|( uint8_t(color>>8));
	wr_comm_last(RAMWR);
	spi_write(&_c,2);
}

void TFT9341::printChar(byte c, int x, int y)
{
	uint16_t fcolor = (uint8_t(_fgc & 0xFF) <<8 )|( uint8_t(_fgc>>8));
	uint16_t bcolor = (uint8_t(_bgc & 0xFF) <<8 )|( uint8_t(_bgc>>8));	
	
	byte i,ch;
	word j;
	word temp; 
  	TFT_CS_LOW;
	if (!_transparent)	{		
		setXY(x,y,x+cfont.x_size-1,y+cfont.y_size-1);
		wr_comm_last(RAMWR);
		
		
		temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;
		for(j=0;j<((cfont.x_size/8)*cfont.y_size);j++) {
			ch=*(&cfont.font[temp]);
			for(i=0;i<8;i++) {   
				if((ch&(1<<(7-i)))!=0) {					 
					 spi_write(&fcolor,2);
				}  else {
					 spi_write(&bcolor,2);
				}   
			}
			temp++;
		}
	} else {
		temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;
		for(j=0;j<cfont.y_size;j++)  {
			for (int zz=0; zz<(cfont.x_size/8); zz++) {
				ch=*(&cfont.font[temp+zz]); 
				for(i=0;i<8;i++) {   
					setXY(x+i+(zz*8),y+j,x+i+(zz*8)+1,y+j+1);
					wr_comm_last(RAMWR);
					if((ch&(1<<(7-i)))!=0) {
						 spi_write(&fcolor,2);
					} 
				}
			}
			temp+=(cfont.x_size/8);
		}
	}
	TFT_CS_HIGH;
}

void TFT9341::print(String st, int x, int y, int deg)
{
	char buf[st.length()+1];
    st.toCharArray(buf, st.length()+1);
	print(buf, x, y, deg);
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
    word color = getColor();
    if (!_transparent)
    {
        int fontHeight = getFontHeight();
        setColor(getBackColor());
        fillRect(x, y, x + fontChar.xDelta+1, y + fontHeight);
        setColor(color);
    }
    
    uint16_t fcolor = (uint8_t(_fgc & 0xFF) <<8 )|( uint8_t(_fgc>>8));
    
    tempPtr = fontChar.dataPtr;
    
    // draw Glyph
   	//*P_CS &= ~B_CS;
     TFT_CS_LOW; 
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
					wr_comm_last(RAMWR);                                
                    spi_write(&fcolor,2);
                } 
                else
                {
                    //setPixel(bcolorr, bcolorg, bcolorb);
                }
                mask >>= 1;
             }
          }
      }
      TFT_CS_HIGH;
    
      
    return fontChar.xDelta;
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
   uint16_t fcolor = (uint8_t(_fgc & 0xFF) <<8 )|( uint8_t(_fgc>>8));
   uint16_t bcolor = (uint8_t(_bgc & 0xFF) <<8 )|( uint8_t(_bgc>>8));
    
   TFT_CS_LOW;
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
                wr_comm_last(RAMWR);                                
                spi_write(&fcolor,2);
            } 
            else
            {
                if (!_transparent)
                {
                    setXY(newX,newY,newX,newY);
	                wr_comm_last(RAMWR);                                
    	            spi_write(&bcolor,2);

                }                
            }
            mask >>= 1;
         }
      }
   }

   TFT_CS_HIGH; 
      	
   return fontChar.xDelta;
}


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

void TFT9341::rotateChar(byte c, int x, int y, int pos, int deg)
{
 	byte i,j,ch;
	word temp; 
	int newx,newy;
	double radian;
	radian=deg*0.0175;  
	temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;
	
	uint16_t fcolor = (uint8_t(_fgc & 0xFF) <<8 )|( uint8_t(_fgc>>8));
	uint16_t bcolor = (uint8_t(_bgc & 0xFF) <<8 )|( uint8_t(_bgc>>8));	
	
	TFT_CS_LOW;
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
				wr_comm_last(RAMWR);                                
                
				if((ch&(1<<(7-i)))!=0)   
				{
					spi_write(&fcolor,2);
				} 
				else  
				{
					if (!_transparent)
						spi_write(&bcolor,2);
				}   
			}
		}
		temp+=(cfont.x_size/8);
	}
	TFT_CS_HIGH;
}

void TFT9341::scroll(uint8_t lh, int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
	
	uint8_t r,g,b;
	uint16_t width = x2 - x1+1;
	uint16_t height = y2 - y1+1;
	uint8_t pixels[3];

	TFT_CS_LOW;	
	/*
	for (uint8_t l=0; l<lh; lh++) {
		for (uint16_t y = 1; y < height; y++) {
			setXY(x1, y1+y, x2, y1+y);
			wr_comm_last(0x2E); // read from RAM
			spi_read(pixels,1);
			for (uint16_t w=0;w<width;w++) {
   			    spi_read(pixels,3);		
   			    r = pixels[0];		// Read a RED byte of GRAM			
				g = pixels[1];		// Read a GREEN byte of GRAM
				b = pixels[2];		// Read a BLUE byte of GRAM		
				scanline[w] = ((((r&248)|g>>5))<<8) | ((g&28)<<3|b>>3);
			}
			//ulozeni o patro vyse
			setXY(x1, y1+y-1, x2, y1+y-1);
			wr_comm_last(RAMWR); 
			spi_write(scanline,width * 2);
		}
	}
	TFT_CS_HIGH;
	*/
	//uint16_t fcolor = getColor();
	setColor(VGA_BLACK);
	fillRect(x1,y2-lh,x2,y2);		
	//setColor(fcolor);
}

/*
void TFT9341::readRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t *pcolors)
{
	uint16_t c = w * h;
	uint8_t r,g,b;
	
	TFT_CS_LOW;	
	setXY(x, y, x+w-1, y+h-1);

	wr_comm_last(0x2E); // read from RAM
	spi_read(0x00,  1);
	
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