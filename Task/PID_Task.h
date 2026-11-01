#ifndef __PID_TASK_H
#define __PID_TASK_H

typedef struct {
	float Target;
	float Actual;
	float Lats_Actual;
	float Out;
	
	float Kp;
	float Ki;
	float Kd;
	
	float Error0;
	float Error1;
	float ErrorInt;
	float ErrorIntMax;
	float ErrorIntMin;
	
	float OutMax;
	float OutMin;
	
	float Outoffset;
} PID_t;

#endif
