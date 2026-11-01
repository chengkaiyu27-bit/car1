#include "All_Head.h"
#include "Blue_Task.h"

static uint8_t RxData;
stCirClieTdf Circle;
char Rxbag[20];
uint8_t Rxbag_flag;
BluePID_t bluePID;
BlueEncoder_t BlueEncoder;

void Cirstrtok(char *Rxbag,uint8_t *Rxbag_flag,stCirClieTdf *Circle)
{
	*Rxbag_flag = 0;
	while(Circle->buff[Circle->head]!=','&&Circle->buff[Circle->head]!=']')
	{
		if(Circle->head!=Circle->tail)
		{
			Rxbag[*Rxbag_flag]=Circle->buff[Circle->head];
			*Rxbag_flag = *Rxbag_flag + 1;
			Circle->head=(Circle->head+1)%128;	
		}
		else
		{
			break;
		}
	}
	Rxbag[*Rxbag_flag] = '\0';
	if(Circle->buff[Circle->head]==']')
	{
		Circle->Cir_long--;
	}
	Circle->head=(Circle->head+1)%128;
}

uint8_t dispose_1(void)
{
	if(Circle.Cir_long)
	{
		while(Circle.head!=Circle.tail)
		{
			if(Circle.buff[Circle.head]=='[')
			{
				Circle.head=(Circle.head+1)%128;
				Rxbag_flag=0;
				return 1;
			}
			else
			{
				Circle.head=(Circle.head+1)%128;
			}
		}
	}
	
	return 0;
}

void dispose_2(void)
{
	if(Circle.head!=Circle.tail)
	{
		Cirstrtok(Rxbag,&Rxbag_flag,&Circle);
		if(strcmp(Rxbag,"slider")==0)
		{
			Cirstrtok(Rxbag,&Rxbag_flag,&Circle);
			if(strcmp(Rxbag,"AngleKp")==0)
			{
				Cirstrtok(Rxbag,&Rxbag_flag,&Circle);
				bluePID.Kp = atof(Rxbag);
			}
			else if(strcmp(Rxbag,"AngleKi")==0)
			{
				Cirstrtok(Rxbag,&Rxbag_flag,&Circle);
				bluePID.Ki = atof(Rxbag);
			}
			else if(strcmp(Rxbag,"AngleKd")==0)	
			{
				Cirstrtok(Rxbag,&Rxbag_flag,&Circle);
				bluePID.Kd = atof(Rxbag);
			}
			xQueueOverwrite(Blue_Queue_PIDHandle,&bluePID);
		}
		else if(strcmp(Rxbag,"joystick")==0)
		{
			Cirstrtok(Rxbag,&Rxbag_flag,&Circle);
			BlueEncoder.Speed_free = atoi(Rxbag);
			Cirstrtok(Rxbag,&Rxbag_flag,&Circle);
			BlueEncoder.Speed_Tager = atoi(Rxbag)/25.0f;
			Cirstrtok(Rxbag,&Rxbag_flag,&Circle);
			BlueEncoder.Dif_speed = atoi(Rxbag)/2.0f;
			Cirstrtok(Rxbag,&Rxbag_flag,&Circle);
			BlueEncoder.Dif_speed_free = atoi(Rxbag);
			xQueueOverwrite(Blue_Set_Moto_Queue_PIDHandle,&BlueEncoder);
		}
	}
}

void StartBlueTask(void *argument)
{
	HAL_UART_Receive_IT(&huart2,&RxData,1);
	TickType_t lastWakeTime = xTaskGetTickCount();
	for(;;)
	{
		if(1==dispose_1())
		{
			dispose_2();
		}
//		vTaskDelay(pdMS_TO_TICKS(5));
		vTaskDelayUntil(&lastWakeTime,pdMS_TO_TICKS(10));
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART2)
	{
		if((Circle.tail+1)%128!=Circle.head)
		{
			Circle.buff[Circle.tail] = RxData;
			Circle.tail=(Circle.tail+1)%128;
			if(RxData==']')
			{
				Circle.Cir_long++;
			}
			HAL_UART_Receive_IT(&huart2,&RxData,1);
			
		}
		else
		{
			HAL_UART_Receive_IT(&huart2,&RxData,1);
			HAL_UART_Transmit_IT(huart,(uint8_t *)"too fast",8);
		}
		
	}
}
