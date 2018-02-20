/*******************************************************
Title:			TCS3200D light sensor library
				signal frequency interface
Filename:		'sensor.h'
Author:			Jakub Dudarewicz
Version:		0.1
Created:		12-2017
Target MCU:		Atmel AVR

This code is distributed under the GNU Public License
which can be found at http://www.gnu.org/licenses/gpl.txt
*******************************************************/

#ifndef SENSOR_H_
#define SENSOR_H_

#define SENSOR_PORT		PORTA
#define SENSOR_DDR		DDRA
#define SENSOR_PIN		PINA
#define SENSOR_OUT		5
#define SENSOR_S2		4
#define SENSOR_S3		6
#define SENSOR_OVF		-1

typedef enum{
	RED,
	BLUE,
	CLEAR,
	GREEN
}SENSOR_COLOR;
//filter color identification

int sensorScan(SENSOR_COLOR color, int accuracy);

#endif /* SENSOR_H_ */