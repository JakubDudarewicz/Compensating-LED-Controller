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
/*
void getGoal(uint8_t *rgoal, uint8_t *ggoal, uint8_t *bgoal, uint32_t time){
	uint32_t sunrise = 16800, sunset = 77580;
}
*/

PID rPID, gPID, bPID;
int rgoal = 400, ggoal = 250, bgoal = 150;
uint32_t time;