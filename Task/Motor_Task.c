#include "All_Head.h"
#include "Motor_Task.h"

emMotorWhoTdf Motor_who;

stMotorStateTdf MotorState;

void set_Motor(emMotorWhoTdf who,int16_t speed)
{
	switch(who)
	{
		case left:
		{
			if(speed>0)
			{
				HAL_GPIO_WritePin(Left1_GPIO_Port,Left1_Pin,GPIO_PIN_SET);
				HAL_GPIO_WritePin(Left2_GPIO_Port,Left2_Pin,GPIO_PIN_RESET);
				if(speed>40)
				{
					speed = 30;
				}
				TIM2->CCR1 = speed;
			}
			else if(speed<0)
			{
				HAL_GPIO_WritePin(Left1_GPIO_Port,Left1_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(Left2_GPIO_Port,Left2_Pin,GPIO_PIN_SET);
				if(speed<-400)
				{
					speed = -30;
				}
				TIM2->CCR1 = -speed;
			}
			break;
		}
		case righ:
		{
			if(speed>0)
			{
				HAL_GPIO_WritePin(Righ1_GPIO_Port,Righ1_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(Righ2_GPIO_Port,Righ2_Pin,GPIO_PIN_SET);
				if(speed>40)
				{
					speed = 30;
				}
				TIM2->CCR2 = speed;
			}
			else if(speed<0)
			{
				HAL_GPIO_WritePin(Righ1_GPIO_Port,Righ1_Pin,GPIO_PIN_SET);
				HAL_GPIO_WritePin(Righ2_GPIO_Port,Righ2_Pin,GPIO_PIN_RESET);
				if(speed<-40)
				{
					speed = -30;
				}
				TIM2->CCR2 = -speed;
			}
			break;
		}
		default:
			break;
	}
}

void StartMotorTask(void *argument)
{
	float Receive=0;
//	BlueMotor_t BlueMotor;
	TickType_t lastwait = xTaskGetTickCount();
	for(;;)
	{
		if(xEventGroupGetBits(Run_EventHandle)&STOP_FLAG)
		{
			if(MotorState.LeftPWM>0)
			{
				MotorState.LeftPWM = 0;
			}
			else if(MotorState.LeftPWM<0)
			{
				MotorState.LeftPWM = 0;
			}
			if(MotorState.RighPWM>0)
			{
				MotorState.RighPWM = 0;
			}
			else if(MotorState.RighPWM<0)
			{
				MotorState.RighPWM = 0;
			}
			set_Motor(left,MotorState.LeftPWM);
			set_Motor(righ,MotorState.RighPWM );
		}
		else
		{
//			if(xQueueReceive(BlueQueue_MotorHandle,&BlueMotor,0)==pdTRUE)
//			{
//				MotorState.DifPWM = BlueMotor.DifPWM;
//			}
			if(xQueueReceive(PID_Queue_MotorHandle,&Receive,0)==pdTRUE)
			{
				MotorState.AvePWM = -(int16_t)Receive;
			}
			if(xQueueReceive(PID_Turn_Queue_MotoHandle,&Receive,0)==pdTRUE)
			{
				MotorState.DifPWM = (int16_t)Receive;
			}
			MotorState.LeftPWM = MotorState.AvePWM + MotorState.DifPWM/2.0f;
			MotorState.RighPWM = MotorState.AvePWM - MotorState.DifPWM/2.0f;
			set_Motor(left,MotorState.LeftPWM);
			set_Motor(righ,MotorState.RighPWM );
		}
		
		vTaskDelayUntil(&lastwait,pdMS_TO_TICKS(10));
	}
}
