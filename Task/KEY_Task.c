#include "KEY_Task.h"
#include "All_Head.h"
// ========== 定义按键状态变量 ==========
volatile emKey_Name_t key_name = Key_none;
uint8_t k1_state, k2_state, k3_state, k4_state;
uint8_t k1_last_state, k2_last_state, k3_last_state, k4_last_state;

// ========== 定义全局变量 ==========
volatile e_ControlMode g_ControlMode = MODE_IDLE;
float g_PresetSpeed = 25.0f;      // 定速值：25 cm/s（比较安全，可自行调整）
float g_PresetDistance = 150.0f;  // 定距离值：150 cm（1.5米）

// 原有按键状态变量...
extern volatile uint32_t time_1ms; // 用于去抖，但可忽略
void key_get(void)
{
	k1_state = HAL_GPIO_ReadPin(K1_GPIO_Port,K1_Pin);
	k2_state = HAL_GPIO_ReadPin(K2_GPIO_Port,K2_Pin);
	k3_state = HAL_GPIO_ReadPin(K3_GPIO_Port,K3_Pin);
	k4_state = HAL_GPIO_ReadPin(K4_GPIO_Port,K4_Pin);
	if(!k1_state&k1_last_state)
	{
		key_name = Key_1;
	}
	else if(!k2_state&k2_last_state)
	{
		key_name = Key_2;
	}
	else if(!k3_state&k3_last_state)
	{
		key_name = Key_3;
	}
	else if(!k4_state&k4_last_state)
	{
		key_name = Key_4;
	}
	k1_last_state = k1_state;
	k2_last_state = k2_state;
	k3_last_state = k3_state;
	k4_last_state = k4_state;
}

void StartKEYTask(void *argument)
{
    for(;;)
    {
        key_get(); // 获取按键状态（原有函数）

        // =========================================================
        // 按键1：启动 / 停止（原有逻辑，增加模式复位）
        // =========================================================
        if(key_name == Key_1)
        {
            if(xEventGroupGetBits(Run_EventHandle) & RUN_FLAG)
            {
                // 当前是运行态 → 切换到停止
                xEventGroupClearBits(Run_EventHandle, RUN_FLAG); 
                xEventGroupSetBits(Run_EventHandle, STOP_FLAG);
                g_ControlMode = MODE_IDLE; // 停止时自动退出任何模式，防止意外启动
            }
            else
            {
                // 当前是停止态 → 切换到运行
                xEventGroupClearBits(Run_EventHandle, STOP_FLAG); 
                xEventGroupSetBits(Run_EventHandle, RUN_FLAG);
                // 注意：进入运行态后，默认是 MODE_IDLE，需要按 K2 或 K3 才能走
            }
            key_name = Key_none;
        }

        // =========================================================
        // 按键2：定速模式（仅在运行状态下生效）
        // =========================================================
        else if(key_name == Key_2)
        {
            // 检查是否处于 RUN 状态（防止在停止状态下误触发）
            if(xEventGroupGetBits(Run_EventHandle) & RUN_FLAG)
            {
                g_ControlMode = MODE_SPEED;
                // 注意：这里只是设定了模式，目标值 g_PresetSpeed 会在 PID_Task 中读取
                // 无需额外操作
            }
            key_name = Key_none;
        }

        // =========================================================
        // 按键3：定距离模式（仅在运行状态下生效）
        // =========================================================
        else if(key_name == Key_3)
        {
            if(xEventGroupGetBits(Run_EventHandle) & RUN_FLAG)
            {
                g_ControlMode = MODE_DISTANCE;
                
                // 【关键】重置累计距离，确保从当前位置开始计程
                // 需要引用 Encoder_Task 中的全局变量
                extern float Total_Distance_CM;
                Total_Distance_CM = 0.0f; 
                
                // 同时重置距离环的积分项（防止上次残留积分导致猛冲）
                // 注意：DistPID 在 PID_Task 中，我们稍后会在 PID_Task 中处理模式切换清理
            }
            key_name = Key_none;
        }

        // 按键4 可以保留作为“停止/退出模式”的备用键，或者不用
        else if(key_name == Key_4)
        {
            // 比如：强制退出所有模式，立即减速停止
            g_ControlMode = MODE_IDLE;
            key_name = Key_none;
        }

        vTaskDelay(pdMS_TO_TICKS(50)); // 50ms 扫描一次按键
    }
}