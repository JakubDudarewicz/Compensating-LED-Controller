
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

uint8_t getInt(char *preamble, int limit){
	char buf = NOKEY, string[20];
	int num = 0;
	while(1){
		buf = keyScan();
		if(buf != NOKEY){
			switch(buf){
				case AKEY:
					lcd_clrscr();
					return num;
				case BKEY:
				case CKEY:
				case DKEY:
				case STARKEY:
				case HASHKEY:
					break;
				default:
					num *= 10;
					num += getKeyInt(buf);
			}
		}
		lcd_clrscr();
		if (num > limit) return limit;
		lcd_puts(preamble);
		if (num)
		{
			itoa(num, string, 10);
			lcd_puts(string);
		}
		_delay_ms(100);
	}
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

int getKeyInt(uint8_t keyCode){
	switch(keyCode){
		case DKEY:
		return 0;
		break;
		case CKEY:
		return 0;
		break;
		case BKEY:
		return 0;
		break;
		case AKEY:
		return 0;
		break;
		case ZEROKEY:
		return 0;
		break;
		case ONEKEY:
		return 1;
		break;
		case TWOKEY:
		return 2;
		break;
		case THREEKEY:
		return 3;
		break;
		case FOURKEY:
		return 4;
		break;
		case FIVEKEY:
		return 5;
		break;
		case SIXKEY:
		return 6;
		break;
		case SEVENKEY:
		return 7;
		break;
		case EIGHTKEY:
		return 8;
		break;
		case NINEKEY:
		return 9;
		break;
		case HASHKEY:
		return 0;
		break;
		case STARKEY:
		return 0;
		break;
		default:
		return 0;
	}
}