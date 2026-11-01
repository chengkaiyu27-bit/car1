#include "All_Head.h"

void StartOLEDTask(void *argument)
{
	OLED_Init();
	
	stStartCoder_CenterTdf StartCoder_Center;
	
	OLED_Clear();
    for(;;)
    {
//		OLED_Printf(0,0,OLED_8X16,"MPU=%d",uxTaskGetStackHighWaterMark(OLEDTaskHandle));
//		OLED_Printf(0,16,OLED_8X16,"MPU=%d",uxTaskGetStackHighWaterMark(LEDTaskHandle));
//		OLED_Printf(0,32,OLED_8X16,"MPU=%d",uxTaskGetStackHighWaterMark(KEYTaskHandle));
//		OLED_Printf(0,48,OLED_8X16,"MPU=%d",uxTaskGetStackHighWaterMark(MPU6050TaskHandle));
//		OLED_Printf(0,0,OLED_8X16,"MPU=%d",uxTaskGetStackHighWaterMark(MotorTaskHandle));
//		OLED_Printf(0,16,OLED_8X16,"MPU=%d",uxTaskGetStackHighWaterMark(EncoderTaskHandle));
//		OLED_Printf(0,32,OLED_8X16,"MPU=%d",uxTaskGetStackHighWaterMark(Coder_CenterTasHandle));
//		OLED_Printf(0,48,OLED_8X16,"MPU=%d",uxTaskGetStackHighWaterMark(BlueTaskHandle));
//		OLED_Printf(0,0,OLED_8X16,"MPU=%d",uxTaskGetStackHighWaterMark(PID_TaskHandle));
		if(xQueueReceive(Queue_OLEDHandle,&StartCoder_Center,0)!=pdTRUE)
		{
			continue;
		}
		OLED_Printf(0,0,OLED_8X16,"AX%+06d",StartCoder_Center.MPUData.AX);
		OLED_Printf(0,16,OLED_8X16,"AY%+06d",StartCoder_Center.MPUData.AY);
		OLED_Printf(0,32,OLED_8X16,"AZ%+06d",StartCoder_Center.MPUData.AZ);
		OLED_Printf(64,0,OLED_8X16,"GX%+06d",StartCoder_Center.MPUData.GX);
		OLED_Printf(64,16,OLED_8X16,"GY%+06d",StartCoder_Center.MPUData.GY);
		OLED_Printf(64,32,OLED_8X16,"GZ%+06d",StartCoder_Center.MPUData.GZ);
		OLED_Printf(0,48,OLED_8X16,"Ang:%6.2f",StartCoder_Center.MPUData.Angle);
		OLED_Printf(0,48,OLED_8X16,"L%+06d",StartCoder_Center.Encoder.left_speed);
		OLED_Printf(64,48,OLED_8X16,"R%+06d",StartCoder_Center.Encoder.righ_speed);
		
		
//		printf("[plot,%d,%d,%d]",StartCoder_Center.MPUData.GX,StartCoder_Center.MPUData.GY,StartCoder_Center.MPUData.GZ);
//		printf("[plot,%d,%d,%d]",StartCoder_Center.MPUData.AX,StartCoder_Center.MPUData.AY,StartCoder_Center.MPUData.AZ);
//		printf("[plot,%d,%d,%d]",StartCoder_Center.MPUData.GX,StartCoder_Center.MPUData.GY,StartCoder_Center.MPUData.GZ);
//		printf("[plot,%f]",StartCoder_Center.MPUData.Angle);
		
	    OLED_Update();
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}
