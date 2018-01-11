/*
 * sensor.h
 *
 * Created: 2018-01-11 7:11:30 PM
 *  Author: Jakub Dudarewicz
 */ 

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

int sensorscan(SENSOR_COLOR color, int accuracy);

#endif /* SENSOR_H_ */