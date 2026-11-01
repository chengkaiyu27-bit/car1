#include "All_Head.h"


void StartMPU6050Task(void *argument)
{
	vTaskDelay(pdMS_TO_TICKS(10));
	MPU6050_Init();
	stMPUDataTdf MPUData;
	float Alpha = 0.01;
	float AngleAcc,AngleGyro;
	TickType_t lastwait = xTaskGetTickCount();
//	xEventGroupClearBits(Run_EventHandle, STOP_FLAG); 
//	xEventGroupSetBits(Run_EventHandle,RUN_FLAG);
	for(;;)
	{
		MPU6050_GetData(&MPUData);
		AngleAcc = -atan2(MPUData.AX,MPUData.AZ)/3.14159*180.0f;
		AngleGyro = MPUData.Angle + MPUData.GY/32768.0f*20.0f;
		MPUData.Angle = Alpha*AngleAcc+(1.0-Alpha)*AngleGyro;

		if(MPUData.Angle>50||MPUData.Angle<-50)
		{
			xEventGroupClearBits(Run_EventHandle, RUN_FLAG); 
			xEventGroupSetBits(Run_EventHandle,STOP_FLAG);
		}
		xQueueOverwrite(MPU_QueueHandle,&MPUData);
		vTaskDelayUntil(&lastwait,pdMS_TO_TICKS(10));
	}
}
