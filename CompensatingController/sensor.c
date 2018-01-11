/*
 * sensor.c
 *
 * Created: 2018-01-11 7:11:01 PM
 *  Author: Jakub Dudarewicz
 */ 
 
#include <avr/io.h>
#include "sensor.h"

 int sensorScan(SENSOR_COLOR color, int accuracy)
 {
	 int buf = 0, i = 0;
	 TIFR0 |= _BV(TOV0);
	 switch (color)
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
		 while (SENSOR_PIN & _BV(SENSOR_OUT));
		 while (!(SENSOR_PIN & _BV(SENSOR_OUT))) buf++;
		 i++;
	 }
	 SENSOR_PORT = 0;
	 if (i < 2) return SENSOR_OVF;
	 else return buf / i;
 }