/*******************************************************
Title:			Ambient Light Compensating LED Controller
				menu structure and display
Filename:		'menu.h'
Author:			Jakub Dudarewicz
Version:		0.1
Created:		12-2017
Target MCU:		Atmel AVR

This code is distributed under the GNU Public License
which can be found at http://www.gnu.org/licenses/gpl.txt
*******************************************************/

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

//item of a linked list
typedef struct MenuItem{
	struct MenuItem *parentItem;
	struct MenuItem *prevItem;
	struct MenuItem *nextItem;
	char itemName[15];
	void (*menuPressed)();
}MenuItem;

//menu scrolling direction
typedef enum MenuDirection{
	UP, DOWN
}MenuDirection;

MenuItem *selectedItem; //currently active item
MenuDirection menuDirection;

void displayMenu2Row();
void displayMenu1Row();
void nextMenu();
void prevMenu();
void parentMenu();
void appendItem();