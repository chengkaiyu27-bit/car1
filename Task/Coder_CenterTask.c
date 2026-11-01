#include "All_Head.h"


void StartCoder_CenterTask(void *argument)
{
	TickType_t lastwait = xTaskGetTickCount();
	stStartCoder_CenterTdf StartCoder_Center;
	for(;;)
	{
		xQueueReceive(MPU_QueueHandle,&StartCoder_Center.MPUData,0);
		xQueueReceive(Encoder_QueueHandle,&StartCoder_Center.Encoder,0);
		
		xQueueOverwrite(Queue_OLEDHandle,&StartCoder_Center);
		xQueueOverwrite(Queue_PIDHandle,&StartCoder_Center);
		vTaskDelayUntil(&lastwait,pdMS_TO_TICKS(10));
	}
}
