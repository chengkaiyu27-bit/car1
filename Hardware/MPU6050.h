#ifndef __MPU6050_H
#define __MPU6050_H
#include "main.h"
#include "MyI2C.h"
#include "MPU6050_Reg.h"

typedef struct 
{
	int16_t AX;
	int16_t AY;
	int16_t AZ;
	int16_t GX;
	int16_t GY;
	int16_t GZ;
	float Angle;
}
stMPUDataTdf;

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data);
uint8_t MPU6050_ReadReg(uint8_t RegAddress);

void MPU6050_Init(void);
uint8_t MPU6050_GetID(void);
void MPU6050_GetData(stMPUDataTdf *MPUData);

#endif
