/*
 * main.h
 *
 * Created: 2017-10-18 3:49:18 PM
 *  Author: Jakub Dudarewicz
 */ 


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

#define BLUEPWM OCR2A
#define GREENPWM OCR2B
#define REDPWM OCR1A

#define ACCURACY 1000
#define BUFLENGTH 20
#define LOOKUPLENGTH 9

void setRGB();
void toggleLight();
void blinkLight();
void displayTime();
void displayLight();
int input(int limit);
void PIDedit();
void goalEdit();
void LEDControl();
void limitAdd8bit(volatile uint8_t *PWMCHANNEL, int value, int limit);
void limitAdd16bit(volatile uint16_t *PWMCHANNEL, int value, int limit);
void setNightGoal();
void setDayGoal();
void setNightBright();
void setDayBright();

PID rPID, gPID, bPID;
float dayBright = 2, nightBright = 1;
int nrgoal = 510, nggoal = 386, nbgoal = 282;
int drgoal = 510, dggoal = 494, dbgoal = 476;
int rgoal = 510, ggoal = 494, bgoal = 476;
int dayLookupRow = 5, nightLookupRow = 7;
const int lookup[LOOKUPLENGTH + 1][4] = {
	{6, 255, 18, 0},
	{10, 255, 68, 0},
	{20, 255, 137, 14},
	{30, 255, 178, 110},
	{40, 255, 206, 167},
	{50, 255, 229, 206},
	{60, 255, 247, 238},
	{65, 255, 255, 255},
	{70, 243, 243, 255},
	{90, 210, 223, 225}
};
char buf, string[16];
int h, m, s = 0, sbuf = -1;
unsigned char refreshPending = TRUE, day = TRUE;
uint32_t time;