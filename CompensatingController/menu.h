
/*
 * menu.h
 *
 * Created: 2017-10-16 3:10:40 PM
 *  Author: Jakub Dudarewicz
 */ 

#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include "RTC.h"
#include "customCharacter.h"

#define UPARROW 0
#define DOWNARROW 1

#define TRUE 1
#define FALSE 0

typedef struct MenuItem{
	struct MenuItem *parentItem;
	struct MenuItem *prevItem;
	struct MenuItem *nextItem;
	char itemName[15];
	void (*menuDisplay)();
	void (*menuPressed)();
}MenuItem;

typedef enum MenuDirection{
	UP, DOWN
}MenuDirection;

MenuItem *selectedItem;
MenuDirection menuDirection;

void displayMenu();
void nextMenu();
void prevMenu();
void parentMenu();
void appendItem();