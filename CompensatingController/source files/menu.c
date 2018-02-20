/*******************************************************
Title:			Ambient Light Compensating LED Controller
				menu structure and display
Filename:		'menu.c'
Author:			Jakub Dudarewicz
Version:		0.1
Created:		12-2017
Target MCU:		Atmel AVR

This code is distributed under the GNU Public License
which can be found at http://www.gnu.org/licenses/gpl.txt
*******************************************************/

#include "menu.h"

void displayMenu2Row(){
	//display two rows of list, with respect to scrolling direction
	//scrolling direction = last move (up or down)
	lcd_clrscr();
	if(((menuDirection == DOWN) & (selectedItem->nextItem != NULL)) |
	   ((menuDirection == UP) & (selectedItem->prevItem == NULL)))
	{
		lcd_goto_xy(0,0);
		lcd_putc('-');
		lcd_puts(selectedItem->itemName);
		lcd_goto_xy(1,1);
		lcd_puts(selectedItem->nextItem->itemName);
	}
	if(((menuDirection == UP) & (selectedItem->prevItem != NULL)) |
	   ((menuDirection == DOWN) & (selectedItem->nextItem == NULL)))
	{
		lcd_goto_xy(1,0);
		lcd_puts(selectedItem->prevItem->itemName);
		lcd_goto_xy(0,1);
		lcd_putc('-');
		lcd_puts(selectedItem->itemName);
	}
	lcd_goto_xy(15, 0);
	lcd_putc(UPARROW); //custom character no
	lcd_goto_xy(15, 1);
	lcd_putc(DOWNARROW); //custom character no
}

void displayMenu1Row(){
	char string[16];

	lcd_clrscr();

	itoa(getTime(HOURS), string, 10);
	lcd_puts(string);
	lcd_putc(':');
	itoa(getTime(MINUTES), string, 10);
	lcd_puts(string);
	lcd_putc(':');
	itoa(getTime(SECONDS), string, 10);
	lcd_puts(string);
	//display time

	lcd_goto_xy(0, 1);

	lcd_puts(selectedItem->itemName);
	//display item description
}

void nextMenu(){
	if(selectedItem->nextItem != NULL) selectedItem = selectedItem->nextItem;
}

void prevMenu(){
	if(selectedItem->prevItem != NULL) selectedItem = selectedItem->prevItem;
}

void parentMenu(){
	if(selectedItem->parentItem != NULL) selectedItem = selectedItem->parentItem;
}

void appendItem(char* newName, void (*menuFunction)()){
	MenuItem *newItem = malloc(sizeof(MenuItem));
	if(selectedItem != NULL)
	{
		selectedItem->nextItem = newItem;
		newItem->prevItem = selectedItem;
	}
	else
	{
		newItem->prevItem = NULL;
		//list termination at the beggining
	}
	newItem->nextItem = NULL;
	//list termination at the end
	selectedItem = newItem;
	strcpy(selectedItem->itemName, newName);
	selectedItem->menuPressed = menuFunction;
	//pointer to function that stores menu functionality
}