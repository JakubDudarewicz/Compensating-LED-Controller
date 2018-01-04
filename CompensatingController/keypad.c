
/*
 * keypad.c
 *
 * Created: 2017-10-16 7:24:27 PM
 *  Author: Jakub Dudarewicz
 */
  
#include "keypad.h"

void keyInit(){
	KEYDDR = 0x00;
	KEYPORT = 0x00;
}

uint8_t keyScan(){
	int i, j;
	for (i = 0; i < 4; i++)
	{
		KEYDDR = _BV(i);
		KEYPORT = _BV(i);
		__asm("nop");
		for (j = 0; j < 4; j++)
		{
			if (KEYPIN & _BV(j + 4)){
				_delay_ms(50);
				while(KEYPIN & _BV(j + 4)){}
				KEYPORT = 0x00;
				KEYDDR = 0x00;
				return (i * 4) + j;
			}
		}
	}
	KEYPORT = 0x00;
	KEYDDR = 0x00;
	return NOKEY;
}

uint8_t get8int(char *preamble){
	char buf = NOKEY;
	char sbuf[] = {' ', ' ', ' '};
	int i = 0;
	lcd_command(_BV(LCD_DISPLAYMODE) | _BV(LCD_DISPLAYMODE_ON) | _BV(LCD_DISPLAYMODE_BLINK));
	lcd_clrscr();
	lcd_puts(preamble);
	while((buf != AKEY) | (i <= 3)){
		buf = keyScan();
		if(buf != NOKEY){
			switch(buf){
				case AKEY:
					lcd_clrscr();
					return atoi(sbuf);
				case BKEY:
					break;
				case CKEY:
					break;
				case DKEY:
					break;
				default:
					sbuf[i++] = getKeyChar(buf);
			}
			lcd_clrscr();
			lcd_puts(preamble);
			for (int j = 0; j < i; j++) lcd_putc(sbuf[j]);
		}
		_delay_ms(100);
	}
	lcd_clrscr();
	return atoi(sbuf);
}

char getKeyChar(uint8_t keyCode){
	switch(keyCode){
		case DKEY:
		return 'D';
		break;
		case CKEY:
		return 'C';
		break;
		case BKEY:
		return 'B';
		break;
		case AKEY:
		return 'A';
		break;
		case ZEROKEY:
		return '0';
		break;
		case ONEKEY:
		return '1';
		break;
		case TWOKEY:
		return '2';
		break;
		case THREEKEY:
		return '3';
		break;
		case FOURKEY:
		return '4';
		break;
		case FIVEKEY:
		return '5';
		break;
		case SIXKEY:
		return '6';
		break;
		case SEVENKEY:
		return '7';
		break;
		case EIGHTKEY:
		return '8';
		break;
		case NINEKEY:
		return '9';
		break;
		case HASHKEY:
		return '#';
		break;
		case STARKEY:
		return '*';
		break;
		default:
		return '$';
	}
}