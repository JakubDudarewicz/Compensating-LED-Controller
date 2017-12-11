/*
 * CompensatingController.c
 *
 * Created: 2017-10-15 11:51:47 AM
 * Author : Jakub Dudarewicz
 */

#define F_CPU 1000000UL

#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include "keypad.h"
#include "hd44780.h"
#include "hedgehog.h"
#include "customCharacter.h"
#include "menu.h"

#define REDPWM OCR2A
#define GREENPWM OCR1A
#define BLUEPWM OCR1B

void setRGB();
void toggleLight();
void blinkLight();

int main(void)
{
	MCUCR |= _BV(JTD);
	MCUCR |= _BV(JTD); // disable JTAG to obtain control of PORTC as general I/O

	TCCR1A = _BV(WGM10) | _BV(COM1A1) | _BV(COM1B1);
	TCCR1B = _BV(WGM12) | _BV(CS10);
	OCR1A = 0x90;
	OCR1B = 0x90;
	TCCR2A = _BV(WGM21) | _BV(WGM20) | _BV(COM2A1);
	TCCR2B = _BV(CS20);
	OCR2A = 0x90;
	DDRD = 0xFF;

	char buf;
	unsigned char refreshPending = TRUE;
	lcd_init();
	keyInit();
	writeCustom(UPARROW, upArrow);
	writeCustom(DOWNARROW, downArrow);
	menuDirection = UP;
	appendItem("RGB", &setRGB);
	appendItem("TOGGLE", &toggleLight);
	appendItem("BLINK", &blinkLight);
    while (1) 
    {
		buf = keyScan();
		if (buf != NOKEY)
		{
			refreshPending = TRUE;
			switch (buf)
			{
				case AKEY:
				prevMenu();
				menuDirection = UP;
				break;
				case BKEY:
				nextMenu();
				menuDirection = DOWN;
				break;
				case CKEY:
				if(selectedItem->menuPressed != NULL) selectedItem->menuPressed();
				break;
			}
		}
		if (refreshPending == TRUE)
		{
			displayMenu();
			refreshPending = FALSE;
		}
    }
}

void setRGB(){
	REDPWM = get8int("R:");
	GREENPWM = get8int("G:");
	BLUEPWM = get8int("B:");
}

void toggleLight(){
	DDRD ^= 0xFF;
}

void blinkLight(){
	for (int i = 0; i < 10; i++){
		DDRD ^= 0xFF;
		_delay_ms(200);
	}
}