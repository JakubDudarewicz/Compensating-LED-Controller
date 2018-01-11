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
#include "i2c.h"
#include "RTC.h"
#include "sensor.h"

#define REDPWM OCR2A
#define GREENPWM OCR2B
#define BLUEPWM OCR1A

void setRGB();
void toggleLight();
void blinkLight();
void displayTime();
void displayLight();
int input(int limit);

int main(void)
{
	MCUCR |= _BV(JTD);
	MCUCR |= _BV(JTD); // disable JTAG to obtain control of PORTC as general I/O

	TCCR1A = _BV(WGM10) | _BV(COM1A1);
	TCCR1B = _BV(WGM12) | _BV(CS10);
	TCCR2A = _BV(WGM21) | _BV(WGM20) | _BV(COM2A1) | _BV(COM2B1);
	TCCR2B = _BV(CS20);

	DDRD |= (_BV(5) | _BV(6) | _BV(7));

	REDPWM = 0x80;
	GREENPWM = 0x50;
	BLUEPWM = 0x30;
	
	KEYDDR = 0x00;
	KEYPORT = 0x00;

	char buf;
	unsigned char refreshPending = TRUE;

	i2cInit();
	setTime(0, 0, 0);
	RTCStart();

	lcd_init();
	lcd_clrscr();

	writeCustom(UPARROW, upArrow);
	writeCustom(DOWNARROW, downArrow);
	menuDirection = UP;
	appendItem("RGB", &setRGB);
	appendItem("TOGGLE", &toggleLight);
	appendItem("BLINK", &blinkLight);
	appendItem("TIME", &displayTime);
	appendItem("SENSOR", &displayLight);
	while(1){
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
	REDPWM = getInt("R:", 255);
	GREENPWM = getInt("G:", 255);
	BLUEPWM = getInt("B:", 255);
}

void toggleLight(){
	DDRD ^= _BV(7) | _BV(6) | _BV(5);
}

void blinkLight(){
	for (int i = 0; i < 10; i++){
		DDRD ^= _BV(7) | _BV(6) | _BV(5);
		_delay_ms(200);
	}
}

void displayTime(){
	char string[20];
	lcd_clrscr();
	itoa(getTime(HOURS), string, 10);
	lcd_puts(string);
	lcd_putc(':');
	itoa(getTime(MINUTES), string, 10);
	lcd_puts(string);
	lcd_putc(':');
	itoa(getTime(SECONDS), string, 10);
	lcd_puts(string);
	_delay_ms(2000);
}

void displayLight(){
	int cbuf, rbuf, gbuf, bbuf;
	char string[20];
	cbuf = sensorScan(CLEAR, 2000);
	rbuf = sensorScan(RED, 2000);
	gbuf = sensorScan(GREEN, 2000);
	bbuf = sensorScan(BLUE, 2000);
	lcd_clrscr();
	itoa(cbuf, string, 10);
	lcd_puts(string);
	lcd_putc(',');
	itoa(rbuf, string, 10);
	lcd_puts(string);
	lcd_goto_xy(0, 1);
	itoa(gbuf, string, 10);
	lcd_puts(string);
	lcd_putc(',');
	itoa(bbuf, string, 10);
	lcd_puts(string);
	_delay_ms(1000);
}