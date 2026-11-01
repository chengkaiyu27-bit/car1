#ifndef __ALL_HEAD_H
#define __ALL_HEAD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "queue.h"
#include "event_groups.h"

#include "gpio.h"
#include "tim.h"
#include "usart.h"
#include "OLED.h"
#include "MPU6050.h"

#include "Motor_Task.h"
#include "Encoder_Task.h"
#include "Blue_Task.h"
#include "PID_Task.h"


#define RUN_FLAG	0x01
#define STOP_FLAG	(0x01<<1)
// ========== 物理参数配置（单位：厘米、秒） ==========
#define WHEEL_DIAMETER_CM   5.6f        // 轮胎直径 56mm = 5.6cm
#define WHEEL_CIRCUMFERENCE (WHEEL_DIAMETER_CM * 3.1415926f) // ≈ 17.59cm

#define MOTOR_PPR           11.0f       // 编码器基础脉冲（每转11脉冲）
#define GEAR_RATIO          9.6f        // 减速比 9.6:1
#define ENCODER_4X_RES      (MOTOR_PPR * GEAR_RATIO * 4.0f) // = 422.4

#define ENCODER_SAMPLE_TIME 0.05f       // 50ms = 0.05秒

// 速度换算系数：将 50ms 的 CNT 计数直接转换为 cm/s
#define CNT_TO_CMPS (WHEEL_CIRCUMFERENCE / (ENCODER_4X_RES * ENCODER_SAMPLE_TIME))
// = 17.59 / (422.4 × 0.05) ≈ 17.59 / 21.12 ≈ 0.833

typedef enum {
    MODE_IDLE = 0,      // 空闲模式（无目标）
    MODE_SPEED,         // 定速模式
    MODE_DISTANCE       // 定距离模式
} e_ControlMode;

// 外部声明（变量定义放在 KEY_Task.c 中）
extern volatile e_ControlMode g_ControlMode;
extern float g_PresetSpeed;      // 预设目标速度（cm/s）
extern float g_PresetDistance;   // 预设目标距离（cm）

typedef struct
{
	stMPUDataTdf MPUData;		//16
	stEncoderDataTdf Encoder;	//16
}
stStartCoder_CenterTdf;

extern volatile uint32_t time_1ms;

extern osMessageQueueId_t MPU_QueueHandle;
extern osMessageQueueId_t Encoder_QueueHandle;

extern osMessageQueueId_t PID_Queue_MotorHandle;
extern osMessageQueueId_t Blue_Queue_PIDHandle;
extern osMessageQueueId_t Blue_Set_Moto_Queue_PIDHandle;
extern osMessageQueueId_t Encoder_Queue_PIDHandle;
extern osMessageQueueId_t PID_Turn_Queue_MotoHandle;

extern osMessageQueueId_t Queue_OLEDHandle;
extern osMessageQueueId_t Queue_PIDHandle;

extern osEventFlagsId_t Run_EventHandle;

extern osThreadId_t OLEDTaskHandle;
extern osThreadId_t LEDTaskHandle;
extern osThreadId_t KEYTaskHandle;
extern osThreadId_t MPU6050TaskHandle;
extern osThreadId_t MotorTaskHandle;
extern osThreadId_t EncoderTaskHandle;
extern osThreadId_t Coder_CenterTasHandle;
extern osThreadId_t BlueTaskHandle;
extern osThreadId_t PID_TaskHandle;

#endif
