/*******************************************************
Title:			PID control implementation
Filename:		'PID.h'
Author:			Jakub Dudarewicz
Version:		0.1
Created:		01-2018
Target MCU:		Atmel AVR

This code is distributed under the GNU Public License
which can be found at http://www.gnu.org/licenses/gpl.txt
*******************************************************/

#ifndef PID_H_
#define PID_H_

typedef float real_t;

typedef struct{
	real_t pGain, iGain, dGain;
	real_t iState, dState;
	real_t iMax, iMin;
}PID;

void PIDinit(PID *pid,
			 real_t pGain,
			 real_t iGain,
			 real_t dGain,
			 real_t iMax,
			 real_t iMin);

real_t updatePID(PID *pid,
			     real_t goal,
				 real_t currentState);

#endif /* PID_H_ */