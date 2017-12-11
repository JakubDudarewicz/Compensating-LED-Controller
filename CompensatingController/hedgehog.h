/*
 * hedgehog.h
 *
 * Created: 2017-10-15 6:37:16 PM
 *  Author: Jakub Dudarewicz
 */ 


#ifndef HEDGEHOG_H_
#define HEDGEHOG_H_

#include <avr/io.h>
#include "hd44780.h"

#define HEDGEHOG 0
#define LEFTHEDGEHOG 1

void writeHedgehog();
void writeLeftHedgehog();

#endif /* HEDGEHOG_H_ */