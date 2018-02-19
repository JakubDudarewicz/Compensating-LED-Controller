/*******************************************************
Title:			HD44780 custom character library
Filename:		'customCharacter.c'
Author:			Jakub Dudarewicz
Version:		0.1
Created:		12-2017
Target MCU:		Atmel AVR

This code is distributed under the GNU Public License
which can be found at http://www.gnu.org/licenses/gpl.txt
*******************************************************/
 
#include <avr/io.h>
#include "hd44780.h"
#include "hd44780.h"
#include <util/delay.h>

#ifndef CUSTOMCHARACTER_H_
#define CUSTOMCHARACTER_H_

uint8_t characters[8][8];
uint8_t downArrow[8];
uint8_t upArrow[8];
uint8_t jez[8][8];

void writeCustom(uint8_t characterID, uint8_t* lines);
void putPixel(uint8_t x, uint8_t y);
void writeCharacters();
void clearString();
void clearCharacters();
void copyHedgehog();

#endif /* CUSTOMCHARACTER_H_ */