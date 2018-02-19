/*******************************************************
Title:			4x4 matrix keypad interface
Filename:		'keypad.h'
Author:			Jakub Dudarewicz
Version:		0.1
Created:		11-2017
Target MCU:		Atmel AVR

This code is distributed under the GNU Public License
which can be found at http://www.gnu.org/licenses/gpl.txt
*******************************************************/

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