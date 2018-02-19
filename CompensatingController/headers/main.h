/*******************************************************
Title:			Ambient Light Compensating LED Controller
Filename:		'main.h'
Author:			Jakub Dudarewicz
Version:		0.1
Created:		02-2018
Target MCU:		Atmel AVR ATmega324A

This code is distributed under the GNU Public License
which can be found at http://www.gnu.org/licenses/gpl.txt
*******************************************************/

#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include "menu.h"
#include "keypad.h"
#include "hd44780.h"
#include "hedgehog.h"
#include "customCharacter.h"
#include "i2c.h"
#include "RTC.h"
#include "sensor.h"
#include "PID.h"

#define F_CPU 1000000UL

#define BLUEPWM			OCR2A
#define GREENPWM		OCR2B
#define REDPWM			OCR1A

#define ACCURACY		1000
#define BUFLENGTH		20
#define LOOKUPLENGTH	9
#define PGAIN			0.01
#define IGAIN			0.005
#define DGAIN			0.001

void setRGB();
void toggleLight();
void LEDControl();
void limitAdd8bit(volatile uint8_t *PWMCHANNEL, int value, int limit);
void limitAdd16bit(volatile uint16_t *PWMCHANNEL, int value, int limit);
void setNightGoal();
void setDayGoal();
void setNightBright();
void setDayBright();