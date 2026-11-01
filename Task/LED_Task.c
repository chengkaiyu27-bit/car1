#include "All_Head.h"

void StartLEDTask(void *argument)
{
    for(;;)
    {
		if(xEventGroupGetBits(Run_EventHandle)&STOP_FLAG)
		{
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13,GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13,GPIO_PIN_RESET);
		}
		vTaskDelay(pdMS_TO_TICKS(100));
    }
}
