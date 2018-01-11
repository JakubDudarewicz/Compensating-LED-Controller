
/*
 * keypad.h
 *
 * Created: 2017-10-16 7:24:17 PM
 *  Author: Jakub Dudarewicz
 */ 

#define F_CPU 1000000UL

#define KEYPORT PORTB
#define KEYDDR DDRB
#define KEYPIN PINB
#define NOKEY 0xFF

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "hd44780.h"

enum KEYS{ONEKEY, FOURKEY, SEVENKEY, STARKEY,
		  TWOKEY, FIVEKEY, EIGHTKEY, ZEROKEY,
		  THREEKEY, SIXKEY, NINEKEY, HASHKEY,
		  AKEY, BKEY, CKEY, DKEY};

void keyInit();
uint8_t keyScan();
uint8_t getInt(char* preamble, int limit);
char getKeyChar(uint8_t keyCode);
int getKeyInt(uint8_t keyCode);