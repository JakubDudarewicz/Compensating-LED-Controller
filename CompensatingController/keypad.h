
/*
 * keypad.h
 *
 * Created: 2017-10-16 7:24:17 PM
 *  Author: Jakub Dudarewicz
 */ 

#define F_CPU 1000000UL

#define KEYPORT PORTC
#define KEYDDR DDRC
#define KEYPIN PINC
#define NOKEY 0xFF

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "hd44780.h"

enum KEYS{DKEY, HASHKEY, ZEROKEY, STARKEY, CKEY, NINEKEY, EIGHTKEY, SEVENKEY, BKEY, SIXKEY, FIVEKEY, FOURKEY, AKEY, THREEKEY, TWOKEY, ONEKEY};

void keyInit();
uint8_t keyScan();
uint8_t get8int(char* preamble);
char getKeyChar(uint8_t keyCode);