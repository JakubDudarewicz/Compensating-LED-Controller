/*
 * PID.c
 *
 * Created: 2018-01-22 2:04:39 PM
 *  Author: Jakub Dudarewicz
 */ 

#include "PID.h"

real_t updatePID(PID *pid, real_t goal, real_t currentState){
	real_t pOut, iOut, dOut, error;
	error = goal - currentState;

	pOut = pid->pGain * error;

	pid->iState += error;
	if(pid->iState > pid->iMax) pid->iState = pid->iMax;
	if(pid->iState < pid->iMin) pid->iState = pid->iMin;
	iOut = pid->iGain * pid->iState;

	dOut = pid->dGain * (pid->dState - currentState);
	pid->dState = currentState;

	return pOut + iOut + dOut;
}

void PIDinit(PID *pid, real_t pGain, real_t iGain, real_t dGain, real_t iMax, real_t iMin){
	pid->pGain = pGain;
	pid->iGain = iGain;
	pid->dGain = dGain;
	pid->iMax = iMax;
	pid->iMin = iMin;
	pid->iState = iMin + ((iMax - iMin) / 2);
	pid->dState = 0;
}