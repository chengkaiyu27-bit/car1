#ifndef __BLUE_TASK_H
#define __BLUE_TASK_H

typedef struct
{
	char buff[512];
	uint8_t head;
	uint8_t tail;
	uint16_t Cir_long;
}
stCirClieTdf;

typedef struct {
	float Kp;
	float Ki;
	float Kd;
} BluePID_t;

typedef struct {
//	int8_t Angle_free;
//	int8_t Angle_Tager;
	float Speed_free;
	float Speed_Tager;
	float Dif_speed;
	float Dif_speed_free;
	
} BlueEncoder_t;

#endif
