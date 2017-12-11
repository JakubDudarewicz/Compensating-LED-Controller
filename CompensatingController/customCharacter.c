/*
* customCharacter.c
*
* Created: 2017-10-15 6:39:54 PM
*  Author: Jakub Dudarewicz
*/ 

#include "customCharacter.h"

uint8_t characters[8][8];

uint8_t downArrow[8] = {
	0b00000, 0b00000, 0b11111, 0b01110,
	0b00100, 0b00000, 0b00000, 0b00000
};

uint8_t upArrow[8] = {
	0b00000, 0b00000, 0b00000, 0b00100,
	0b01110, 0b11111, 0b00000, 0b00000
};


uint8_t jez[8][8] = {
	{0b00000, 0b00000, 0b00000, 0b00000,
	 0b00000, 0b00010, 0b00011, 0b01011},
	{0b00000, 0b00000, 0b00000, 0b00000,
	 0b10101, 0b11111, 0b11111, 0b11111},
	{0b00000, 0b00000, 0b00000, 0b00000,
	 0b10000, 0b10100, 0b11110, 0b11111},
	{0b00000, 0b00000, 0b00000, 0b00000,
	 0b00000, 0b00000, 0b00000, 0b10000},
	{0b01111, 0b11111, 0b01111, 0b11111,
	 0b11111, 0b11111, 0b00100, 0b00110},
	{0b11111, 0b11111, 0b11111, 0b11111,
	 0b11111, 0b11111, 0b10000, 0b11000},
	{0b11111, 0b11111, 0b11111, 0b11111,
	 0b11111, 0b11111, 0b00100, 0b00110},
	{0b10000, 0b11100, 0b11000, 0b11100,
	 0b10101, 0b11111, 0b01000, 0b01100}
};

uint8_t temp[8];

void writeCustom(uint8_t characterID, uint8_t* lines){
	lcd_command(_BV(LCD_CGRAM)+characterID*8);
	for(int i = 0; i < 8; i++)
	{
		lcd_putc(lines[i]);
	}
	lcd_goto(0);
}

void putPixel(uint8_t x, uint8_t y){
	characters[(uint8_t)(x / 5)][y] |= _BV(4 - (x % 5));
}

void writeCharacters(){
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++)
		{
			temp[j] = characters[i][j];
		}
		writeCustom(i, temp);
	}
}

void clearString(){
	for (int i = 0; i < 64; i++)
	{
		characters[i/8][i%8] = 0;
	}
}

void clearCharacters(){
	for(int j = 0; j < 8; j++) temp[j] = 0;
	for(int i = 0; i < 8; i++) writeCustom(i, temp);
}

void copyHedgehog(){
	for (int i = 0; i < 64; i++) characters[i / 8][i % 8] = jez[i / 8][i % 8];
}