/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "queue.h"
#include "event_groups.h"
#include "All_Head.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for OLEDTask */
osThreadId_t OLEDTaskHandle;
const osThreadAttr_t OLEDTask_attributes = {
  .name = "OLEDTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for LEDTask */
osThreadId_t LEDTaskHandle;
const osThreadAttr_t LEDTask_attributes = {
  .name = "LEDTask",
  .stack_size = 64 * 4,
  .priority = (osPriority_t) osPriorityLow1,
};
/* Definitions for KEYTask */
osThreadId_t KEYTaskHandle;
const osThreadAttr_t KEYTask_attributes = {
  .name = "KEYTask",
  .stack_size = 64 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for MPU6050Task */
osThreadId_t MPU6050TaskHandle;
const osThreadAttr_t MPU6050Task_attributes = {
  .name = "MPU6050Task",
  .stack_size = 98 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal2,
};
/* Definitions for MotorTask */
osThreadId_t MotorTaskHandle;
const osThreadAttr_t MotorTask_attributes = {
  .name = "MotorTask",
  .stack_size = 78 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for EncoderTask */
osThreadId_t EncoderTaskHandle;
const osThreadAttr_t EncoderTask_attributes = {
  .name = "EncoderTask",
  .stack_size = 78 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal1,
};
/* Definitions for Coder_CenterTas */
osThreadId_t Coder_CenterTasHandle;
const osThreadAttr_t Coder_CenterTas_attributes = {
  .name = "Coder_CenterTas",
  .stack_size = 78 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal1,
};
/* Definitions for BlueTask */
osThreadId_t BlueTaskHandle;
const osThreadAttr_t BlueTask_attributes = {
  .name = "BlueTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for PID_Task */
osThreadId_t PID_TaskHandle;
const osThreadAttr_t PID_Task_attributes = {
  .name = "PID_Task",
  .stack_size = 88 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal1,
};
/* Definitions for MPU_Queue */
osMessageQueueId_t MPU_QueueHandle;
const osMessageQueueAttr_t MPU_Queue_attributes = {
  .name = "MPU_Queue"
};
/* Definitions for Encoder_Queue */
osMessageQueueId_t Encoder_QueueHandle;
const osMessageQueueAttr_t Encoder_Queue_attributes = {
  .name = "Encoder_Queue"
};
/* Definitions for Queue_OLED */
osMessageQueueId_t Queue_OLEDHandle;
const osMessageQueueAttr_t Queue_OLED_attributes = {
  .name = "Queue_OLED"
};
/* Definitions for Queue_PID */
osMessageQueueId_t Queue_PIDHandle;
const osMessageQueueAttr_t Queue_PID_attributes = {
  .name = "Queue_PID"
};
/* Definitions for PID_Queue_Motor */
osMessageQueueId_t PID_Queue_MotorHandle;
const osMessageQueueAttr_t PID_Queue_Motor_attributes = {
  .name = "PID_Queue_Motor"
};
/* Definitions for Blue_Queue_PID */
osMessageQueueId_t Blue_Queue_PIDHandle;
const osMessageQueueAttr_t Blue_Queue_PID_attributes = {
  .name = "Blue_Queue_PID"
};
/* Definitions for Blue_Set_Moto_Queue_PID */
osMessageQueueId_t Blue_Set_Moto_Queue_PIDHandle;
const osMessageQueueAttr_t Blue_Set_Moto_Queue_PID_attributes = {
  .name = "Blue_Set_Moto_Queue_PID"
};
/* Definitions for Encoder_Queue_PID */
osMessageQueueId_t Encoder_Queue_PIDHandle;
const osMessageQueueAttr_t Encoder_Queue_PID_attributes = {
  .name = "Encoder_Queue_PID"
};
/* Definitions for PID_Turn_Queue_Moto */
osMessageQueueId_t PID_Turn_Queue_MotoHandle;
const osMessageQueueAttr_t PID_Turn_Queue_Moto_attributes = {
  .name = "PID_Turn_Queue_Moto"
};
/* Definitions for Run_Event */
osEventFlagsId_t Run_EventHandle;
const osEventFlagsAttr_t Run_Event_attributes = {
  .name = "Run_Event"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartOLEDTask(void *argument);
void StartLEDTask(void *argument);
void StartKEYTask(void *argument);
void StartMPU6050Task(void *argument);
void StartMotorTask(void *argument);
void StartEncoderTask(void *argument);
void StartCoder_CenterTask(void *argument);
void StartBlueTask(void *argument);
void StartPIDTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN PREPOSTSLEEP */
__weak void PreSleepProcessing(uint32_t *ulExpectedIdleTime)
{
	
/* place for user code */
	HAL_SuspendTick();
}

__weak void PostSleepProcessing(uint32_t *ulExpectedIdleTime)
{
	
/* place for user code */
	HAL_ResumeTick();
}
/* USER CODE END PREPOSTSLEEP */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of MPU_Queue */
  MPU_QueueHandle = osMessageQueueNew (1, 16, &MPU_Queue_attributes);

  /* creation of Encoder_Queue */
  Encoder_QueueHandle = osMessageQueueNew (1, 16, &Encoder_Queue_attributes);

  /* creation of Queue_OLED */
  Queue_OLEDHandle = osMessageQueueNew (1, 32, &Queue_OLED_attributes);

  /* creation of Queue_PID */
  Queue_PIDHandle = osMessageQueueNew (1, 32, &Queue_PID_attributes);

  /* creation of PID_Queue_Motor */
  PID_Queue_MotorHandle = osMessageQueueNew (1, 4, &PID_Queue_Motor_attributes);

  /* creation of Blue_Queue_PID */
  Blue_Queue_PIDHandle = osMessageQueueNew (1, 12, &Blue_Queue_PID_attributes);

  /* creation of Blue_Set_Moto_Queue_PID */
  Blue_Set_Moto_Queue_PIDHandle = osMessageQueueNew (1, 16, &Blue_Set_Moto_Queue_PID_attributes);

  /* creation of Encoder_Queue_PID */
  Encoder_Queue_PIDHandle = osMessageQueueNew (1, 16, &Encoder_Queue_PID_attributes);

  /* creation of PID_Turn_Queue_Moto */
  PID_Turn_Queue_MotoHandle = osMessageQueueNew (1, 4, &PID_Turn_Queue_Moto_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of OLEDTask */
  OLEDTaskHandle = osThreadNew(StartOLEDTask, NULL, &OLEDTask_attributes);

  /* creation of LEDTask */
  LEDTaskHandle = osThreadNew(StartLEDTask, NULL, &LEDTask_attributes);

  /* creation of KEYTask */
  KEYTaskHandle = osThreadNew(StartKEYTask, NULL, &KEYTask_attributes);

  /* creation of MPU6050Task */
  MPU6050TaskHandle = osThreadNew(StartMPU6050Task, NULL, &MPU6050Task_attributes);

  /* creation of MotorTask */
  MotorTaskHandle = osThreadNew(StartMotorTask, NULL, &MotorTask_attributes);

  /* creation of EncoderTask */
  EncoderTaskHandle = osThreadNew(StartEncoderTask, NULL, &EncoderTask_attributes);

  /* creation of Coder_CenterTas */
  Coder_CenterTasHandle = osThreadNew(StartCoder_CenterTask, NULL, &Coder_CenterTas_attributes);

  /* creation of BlueTask */
  BlueTaskHandle = osThreadNew(StartBlueTask, NULL, &BlueTask_attributes);

  /* creation of PID_Task */
  PID_TaskHandle = osThreadNew(StartPIDTask, NULL, &PID_Task_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the event(s) */
  /* creation of Run_Event */
  Run_EventHandle = osEventFlagsNew(&Run_Event_attributes);
  if (Run_EventHandle != NULL) {
        osEventFlagsSet(Run_EventHandle, STOP_FLAG); 
        // 注意：不要设置 RUN_FLAG，默认只有 STOP_FLAG
    }
	
  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartOLEDTask */
/**
  * @brief  Function implementing the OLEDTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartOLEDTask */
__weak void StartOLEDTask(void *argument)
{
  /* USER CODE BEGIN StartOLEDTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartOLEDTask */
}

/* USER CODE BEGIN Header_StartLEDTask */
/**
* @brief Function implementing the LEDTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartLEDTask */
__weak void StartLEDTask(void *argument)
{
  /* USER CODE BEGIN StartLEDTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartLEDTask */
}

/* USER CODE BEGIN Header_StartKEYTask */
/**
* @brief Function implementing the KEYTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartKEYTask */
__weak void StartKEYTask(void *argument)
{
  /* USER CODE BEGIN StartKEYTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartKEYTask */
}

/* USER CODE BEGIN Header_StartMPU6050Task */
/**
* @brief Function implementing the MPU6050Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMPU6050Task */
__weak void StartMPU6050Task(void *argument)
{
  /* USER CODE BEGIN StartMPU6050Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartMPU6050Task */
}

/* USER CODE BEGIN Header_StartMotorTask */
/**
* @brief Function implementing the MotorTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMotorTask */
__weak void StartMotorTask(void *argument)
{
  /* USER CODE BEGIN StartMotorTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartMotorTask */
}

/* USER CODE BEGIN Header_StartEncoderTask */
/**
* @brief Function implementing the EncoderTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartEncoderTask */
__weak void StartEncoderTask(void *argument)
{
  /* USER CODE BEGIN StartEncoderTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartEncoderTask */
}

/* USER CODE BEGIN Header_StartCoder_CenterTask */
/**
* @brief Function implementing the Coder_CenterTas thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCoder_CenterTask */
__weak void StartCoder_CenterTask(void *argument)
{
  /* USER CODE BEGIN StartCoder_CenterTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartCoder_CenterTask */
}

/* USER CODE BEGIN Header_StartBlueTask */
/**
* @brief Function implementing the BlueTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartBlueTask */
__weak void StartBlueTask(void *argument)
{
  /* USER CODE BEGIN StartBlueTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartBlueTask */
}

/* USER CODE BEGIN Header_StartPIDTask */
/**
* @brief Function implementing the PID_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartPIDTask */
__weak void StartPIDTask(void *argument)
{
  /* USER CODE BEGIN StartPIDTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartPIDTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

