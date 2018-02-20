/*******************************************************
Title:			TCS3200D light sensor library
				signal frequency interface
Filename:		'sensor.c'
Author:			Jakub Dudarewicz
Version:		0.1
Created:		12-2017
Target MCU:		Atmel AVR

This code is distributed under the GNU Public License
which can be found at http://www.gnu.org/licenses/gpl.txt
*******************************************************/
 
#include <avr/io.h>
#include "sensor.h"

 int sensorScan(SENSOR_COLOR color, int accuracy)
 {
	 int buf = 0, i = 0;
	 switch (color) //color filter selection
	 {
		 case CLEAR:
		 SENSOR_PORT = _BV(SENSOR_S2);
		 break;
		 case RED:
		 SENSOR_PORT = 0;
		 break;
		 case GREEN:
		 SENSOR_PORT = _BV(SENSOR_S2) | _BV(SENSOR_S3);
		 break;
		 case BLUE:
		 SENSOR_PORT = _BV(SENSOR_S3);
		 break;
	 }
	 while(buf <= accuracy)
	 {
		 while (SENSOR_PIN & _BV(SENSOR_OUT)); //wai for signal low
		 while (!(SENSOR_PIN & _BV(SENSOR_OUT))) buf++; //count while signal low
		 i++; //increment number of period counts
	 }
	 SENSOR_PORT = 0;
	 if (i < 2) return SENSOR_OVF; //if too dark and measurement unreliable, return error code
	 else return accuracy - (buf / i); //average counts by periods
 }