  ILI9341 TFT SPI library
  
  based on UTFT.cpp - Arduino/chipKit library support for Color TFT LCD Boards
  Original library you can find at http://electronics.henningkarlsen.com/  
 
  and UTFT_DLB extension from https://sites.google.com/site/dlbarduino/
   
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the CC BY-NC-SA 3.0 license.
  Please see the included documents for further information.
 
*********************************************************************************
Pinout: (define in TFT9341.h)
 
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
    Arduino D6   -> 4050 pin 6: 4050 pin 7 -> TFT  D/C
    Arduino D7   -> TFT LED
    Arduino D51  -> 4050 pin 9: 4050 pin 10 -> TFT MOSI
    Arduino D50  -> TFT MISO
    Arduino D52  -> 4050 pin 11: 4050 pin 12 -> TFT SCK

(Arduino M0 -> TFT)

	Arduino MOSI -> TFT MOSI
	Arduino MISO -> TFT MISO
	Arduino SCK	 -> TFT SCK
	Arduino D12  -> TFT CS
	Arduino D11  -> TFT RESET 
	Arduino D10  -> TFT DC
	Arduino D9   -> TFT LED
	
*********************************************************************************
 
Change notes:

	1.0  - initial version
	
	1.1 (16.8.14) - performance improvements
	
	1.2 (5.1.15)  - added function setRotation setRotation - allow rotate display (0=landscape, 1=portrait, 2 = landscape, 3=portrait)
				  - comment out #define FASTSPI in UTFT.h when use ethernet shield
				  - removed readID() from  display init
				  
	1.3 (28.1.16) - Basic Arduino M0 compatibility
	
	2.0beta (18.2.16) - Arduino M0 DMA SPI, fix rotation, 
			      - added vertical scroll, performance improvements
			      - added proportional fonts from from https://sites.google.com/site/dlbarduino/
	
	2.0beta (20.2.16) - added println, fixes
