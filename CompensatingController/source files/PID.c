/*******************************************************
Title:			PID control implementation
Filename:		'PID.c'
Author:			Jakub Dudarewicz
Version:		0.1
Created:		01-2018
Target MCU:		Atmel AVR

This code is distributed under the GNU Public License
which can be found at http://www.gnu.org/licenses/gpl.txt
*******************************************************/

#include "PID.h"

real_t updatePID(PID *pid, real_t goal, real_t currentState){
	//PID output and state calculation
	real_t pOut, iOut, dOut, error;
	error = goal - currentState; //error calculation

	pOut = pid->pGain * error; //proportional block output

	pid->iState += error; //'integration'
	if(pid->iState > pid->iMax) pid->iState = pid->iMax;
	if(pid->iState < pid->iMin) pid->iState = pid->iMin; //integration limit
	iOut = pid->iGain * pid->iState; //integrating block output

	dOut = pid->dGain * (pid->dState - currentState); //derivation
	pid->dState = currentState; //derivating block output

	return pOut + iOut + dOut;
}

void PIDinit(PID *pid, real_t pGain, real_t iGain, real_t dGain, real_t iMax, real_t iMin){
	pid->pGain = pGain;
	pid->iGain = iGain;
	pid->dGain = dGain;
	pid->iMax = iMax;
	pid->iMin = iMin;
	pid->iState = iMin + ((iMax - iMin) / 2); //integration windup cancelling
	pid->dState = 0;
}