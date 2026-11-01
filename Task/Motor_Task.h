#ifndef __MOTOR_TASK_H
#define __MOTOR_TASK_H

typedef enum
{
	left		=0,
	righ,
}
emMotorWhoTdf;

typedef struct
{
	int16_t AvePWM;
	int16_t DifPWM;
	int16_t LeftPWM;
	int16_t RighPWM;
}
stMotorStateTdf;

#endif
