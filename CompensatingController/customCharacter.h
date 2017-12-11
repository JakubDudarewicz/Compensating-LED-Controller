/*
 * customCharacter.h
 *
 * Created: 2017-10-15 6:38:46 PM
 *  Author: Jakub Dudarewicz
 */ 
 
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