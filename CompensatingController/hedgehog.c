/*
* CFile1.c
*
* Created: 2017-10-15 6:36:23 PM
*  Author: Jakub Dudarewicz
*/ 
#include "hedgehog.h"
 

void writeHedgehog(){
	lcd_command(_BV(LCD_CGRAM)+HEDGEHOG*8);
	lcd_putc(0b00010);
	lcd_putc(0b00110);
	lcd_putc(0b01011);
	lcd_putc(0b11110);
	lcd_putc(0b11110);
	lcd_putc(0b11110);
	lcd_putc(0b01111);
	lcd_putc(0b00110);
	lcd_goto(0);
}

void writeLeftHedgehog(){
	lcd_command(_BV(LCD_CGRAM)+LEFTHEDGEHOG*8);
	lcd_putc(0b01000);
	lcd_putc(0b01100);
	lcd_putc(0b11010);
	lcd_putc(0b01111);
	lcd_putc(0b01111);
	lcd_putc(0b01111);
	lcd_putc(0b11110);
	lcd_putc(0b01100);
	lcd_goto(0);
}