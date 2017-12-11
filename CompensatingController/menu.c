
/*
 * menu.c
 *
 * Created: 2017-10-16 3:10:18 PM
 *  Author: Jakub Dudarewicz
 */ 

#include "menu.h"

void displayMenu(){
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
	lcd_putc(UPARROW);
	lcd_goto_xy(15, 1);
	lcd_putc(DOWNARROW);
}

void nextMenu(){
	if(selectedItem->nextItem != NULL) selectedItem = selectedItem->nextItem;
}

void prevMenu(){
	if(selectedItem->prevItem != NULL) selectedItem = selectedItem->prevItem;
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
	}
	newItem->nextItem = NULL;
	selectedItem = newItem;
	strcpy(selectedItem->itemName, newName);
	selectedItem->menuPressed = menuFunction;
}