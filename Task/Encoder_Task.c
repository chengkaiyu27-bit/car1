#include "All_Head.h"
#include "Encoder_Task.h"

#define VSPEED		(44.0*0.05*9.27666f)


// 在 Encoder_Task.c 顶部添加全局变量（用于累加总距离）
float Total_Distance_CM = 0.0f; 

void StartEncoderTask(void *argument)
{
    stEncoderDataTdf Encoder;
    TickType_t lastWakeTime = xTaskGetTickCount();
    
    for(;;)
    {
        // 1. 读取原始计数器值
        int16_t raw_left = (int16_t)__HAL_TIM_GET_COUNTER(&htim3);
        int16_t raw_right = (int16_t)__HAL_TIM_GET_COUNTER(&htim4);

        // 2. 将原始计数值转换为物理速度（单位：cm/s）
        Encoder.left_speed  = raw_left  * CNT_TO_CMPS;
        Encoder.righ_speed  = raw_right * CNT_TO_CMPS;
        Encoder.Ave_speed   = (Encoder.left_speed + Encoder.righ_speed) / 2.0f;
        Encoder.Dif_speed   = Encoder.left_speed - Encoder.righ_speed;

        // 3. 累加物理距离（单位：cm）
        float avg_raw = (raw_left + raw_right) / 2.0f;
        float delta_distance = avg_raw * (WHEEL_CIRCUMFERENCE / ENCODER_4X_RES);
        Total_Distance_CM += delta_distance;

        // 4. 分发数据
        xQueueOverwrite(Encoder_QueueHandle, &Encoder);
        xQueueOverwrite(Encoder_Queue_PIDHandle, &Encoder);

        // 5. 清零计数器
        TIM3->CNT = 0;
        TIM4->CNT = 0;

        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(50));
    }
}
