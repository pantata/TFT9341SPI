  ILI9341 TFT SPI library
  
  based on UTFT.cpp - Arduino/chipKit library support for Color TFT LCD Boards
  Copyright (C)2010-2013 Henning Karlsen. All right reserved
 
  Compatible with other UTFT libraries.
 
  Original library you can find at http://electronics.henningkarlsen.com/
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the CC BY-NC-SA 3.0 license.
  Please see the included documents for further information.
 
*********************************************************************************
Pinout:
 
(Arduino UNO  -> 4050 level shifter  -> TFT)
	4050 pin 1  -> 3.3V, pin 8 GND 
	TFT GND  -> GND
	TFT VCC  -> 3.3V

    Arduino D4   -> 4050 pin 3: 4050 pin 2 ->  TFT  RESET
    Arduino D5   -> 4050 pin 5: 4050 pin 4 -> TFT  CS
    Arduino D6   -> 4050 pin 6: 4050 pin 6 -> TFT  D/C
    Arduino D7   -> TFT LED
    Arduino D11  -> 4050 pin 9: 4050 pin 10 -> TFT MOSI
    Arduino D12  -> TFT MISO
    Arduino D13  -> 4050 pin 11: 4050 pin 12 -> TFT SCK
    
(Arduino MEGA -> 4050 level shifter  -> TFT)   
 
    Arduino D4   -> 4050 pin 3: 4050 pin 2 ->  TFT  RESET
    Arduino D5   -> 4050 pin 5: 4050 pin 4 -> TFT  CS
    Arduino D6   -> 4050 pin 6: 4050 pin 6 -> TFT  D/C
    Arduino D7   -> TFT LED
    Arduino D51  -> 4050 pin 9: 4050 pin 10 -> TFT MOSI
    Arduino D50  -> TFT MISO
    Arduino D52  -> 4050 pin 11: 4050 pin 12 -> TFT SCK
    
*********************************************************************************
 
Change notes:

	1.0  - initial version
	
	1.1 (16.8.14) - performance improvements
	
	1.2 (5.1.15)  - added function setRotation setRotation - allow rotate display (0=landscape, 1=portrait, 2 = landscape, 3=portrait)
				  - comment out #define FASTSPI in UTFT.h when use ethernet shield
				  - removed readID() from  display init
	 
