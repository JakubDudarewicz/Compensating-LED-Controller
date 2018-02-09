/*
 * PID.h
 *
 * Created: 2018-01-22 2:04:49 PM
 *  Author: Jakub Dudarewicz
 */ 

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