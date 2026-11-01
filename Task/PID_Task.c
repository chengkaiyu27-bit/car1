#include "All_Head.h"
#include "PID_Task.h"

#include "All_Head.h"
#include "PID_Task.h"


// ========== PID 结构体定义（原版） ==========
PID_t AnglePID = {
    .Kp = 5.0f,   .Ki = 0.0f,   .Kd = 0.0f,
    .OutMax = 100.0f, .OutMin = -100.0f,
    .ErrorIntMax = 100, .ErrorIntMin = -100,
    .Outoffset = 3
};

PID_t SpeedPID = {
    .Kp = 4.5f,   .Ki = 0.0f,   .Kd = 0.0f,
    .OutMax = 20.0f, .OutMin = -20.0f,
    .ErrorIntMax = 150, .ErrorIntMin = -150,
    .Outoffset = 0
};

PID_t TurnPID = {
    .Kp = 4.0f,   .Ki = 0.0f,   .Kd = 0.0f,
    .OutMax = 50.0f, .OutMin = -50.0f,
    .ErrorIntMax = 20, .ErrorIntMin = -20,
    .Outoffset = 0
};

// ========== 新增距离环 PID ==========
PID_t DistPID = {
    .Kp = 0.8f,   .Ki = 0.02f,  .Kd = 0.0f,
    .OutMax = 30.0f, .OutMin = -30.0f,
    .ErrorIntMax = 50, .ErrorIntMin = -50,
    .Outoffset = 0
};

// 在 PID_Task.c 顶部，三个 PID 定义之后，加上这一行
void PID_Update(PID_t *p)
{
	p->Error1 = p->Error0;
	p->Error0 = p->Target - p->Actual;
	if(p->Ki!=0)
	{
		p->ErrorInt += p->Error0;
		if(p->ErrorInt > p->ErrorIntMax)
		{
			p->ErrorInt = p->ErrorIntMax;
		}
		if(p->ErrorInt < p->ErrorIntMin)
		{
			p->ErrorInt = p->ErrorIntMin;
		}
	}
	else
	{
		p->ErrorInt=0;
	}
	
	
	p->Out = p->Kp * p->Error0 + p->Ki * p->ErrorInt - p->Kd * (p->Actual - p->Lats_Actual);
	if(p->Out > 0)
	{
		p->Out = p->Out + p->Outoffset;
	}
	if(p->Out < 0)
	{
		p->Out = p->Out - p->Outoffset;
	}
	if(p->Out > p->OutMax)
	{
		p->Out = p->OutMax;
	}
	else if(p->Out < p->OutMin)
	{
		p->Out = p->OutMin;
	}
	p->Lats_Actual = p->Actual;
}


// ========== 2. 引用外部变量 ==========
extern volatile e_ControlMode g_ControlMode;
extern float g_PresetSpeed;
extern float g_PresetDistance;
extern float Total_Distance_CM;  // 来自 Encoder_Task

// ========== 3. 修改主任务循环 ==========
void StartPIDTask(void *argument)
{
    stStartCoder_CenterTdf StartCoder_Center;
    TickType_t lastWakeTime = xTaskGetTickCount();
    BluePID_t bluePID;
    stEncoderDataTdf Encoder;
    BlueEncoder_t BlueEncoder;
    
    // 记录上一次的模式，用于检测模式切换，以便清理积分
    e_ControlMode last_mode = MODE_IDLE; 

    for(;;)
    {
        // -------- 停止状态处理（不变） --------
        if(xEventGroupGetBits(Run_EventHandle) & STOP_FLAG)
        {
            AnglePID.ErrorInt=0;
            SpeedPID.ErrorInt=0;
            TurnPID.ErrorInt=0;
            DistPID.ErrorInt = 0; // 距离环积分也要清
            AnglePID.Lats_Actual = 0;
            SpeedPID.Lats_Actual = 0;
            TurnPID.Lats_Actual = 0;
            DistPID.Lats_Actual = 0;
            // 如果停止，强制将模式置为 IDLE，防止启动后乱跑
            g_ControlMode = MODE_IDLE;
        }
        else // -------- 运行状态 --------
        {
            // ---- 3.1 接收蓝牙指令（如果有的话会覆盖按键值） ----
            if(xQueueReceive(Blue_Queue_PIDHandle, &bluePID, 0) == pdTRUE)
            {
                TurnPID.Kp = bluePID.Kp;
                TurnPID.Ki = bluePID.Ki;
                TurnPID.Kd = bluePID.Kd;
            }
            // 注意：蓝牙也可以发目标值，但为了按键优先，我们只在队列里读转向参数，
            // 目标值我们用自己的 g_PresetSpeed 和 g_PresetDistance
            if(xQueueReceive(Blue_Set_Moto_Queue_PIDHandle, &BlueEncoder, 0) == pdTRUE)
            {
                // 蓝牙发来的数据可以覆盖转向目标，但速度/距离目标以按键为主
                TurnPID.Target = BlueEncoder.Dif_speed;
                // 如果你希望蓝牙也能设定定速值，可以在这里更新 g_PresetSpeed = BlueEncoder.Speed_Tager;
            }

            // ---- 3.2 读取编码器反馈（获取当前速度和累计距离） ----
            if(xQueueReceive(Encoder_Queue_PIDHandle, &Encoder, 0) == pdTRUE)
            {
                TurnPID.Actual = Encoder.Dif_speed;
                SpeedPID.Actual = Encoder.Ave_speed;

                // ---- 3.3 【核心】模式切换检测（防积分冲击） ----
                if(g_ControlMode != last_mode) {
                    // 模式发生了变化！
                    // 清空距离环和速度环的积分，防止切模式时猛冲
                    DistPID.ErrorInt = 0;
                    SpeedPID.ErrorInt = 0;
                    DistPID.Lats_Actual = SpeedPID.Actual; // 让微分项不突变
                    last_mode = g_ControlMode;
                }

                // ---- 3.4 根据模式计算 SpeedPID.Target ----
			
                switch(g_ControlMode)
                {
                    case MODE_SPEED:
                        // 定速模式：直接将目标速度设为预设值
                        SpeedPID.Target = g_PresetSpeed;
                        break;

                    case MODE_DISTANCE:
                        // 定距离模式：距离环级联给速度环
                        DistPID.Actual = Total_Distance_CM;
                        DistPID.Target = g_PresetDistance;
                        PID_Update(&DistPID);          // 距离环计算
                        SpeedPID.Target = DistPID.Out; // 距离环输出作为速度目标
                        
                        // 【安全刹车】距离快到时，直接强制衰减，防止过冲
                        float remain = DistPID.Target - DistPID.Actual;
                        if (fabs(remain) < 1.5f && DistPID.Target != 0) {
                            // 剩余距离小于 1.5cm，直接刹停并退出定距模式
                            SpeedPID.Target = 0;
                            g_ControlMode = MODE_IDLE; // 任务完成，退出模式
                            DistPID.ErrorInt = 0;
                        }
                        break;

                    case MODE_IDLE:
                    default:
                        // 空闲模式：目标速度为 0（站在原地）
                        SpeedPID.Target = 0;
                        break;
                }

                // ---- 3.5 更新速度环和转向环 ----
                PID_Update(&TurnPID);
                PID_Update(&SpeedPID);
                
                // 速度环输出给角度环作为目标
                AnglePID.Target = SpeedPID.Out;
                
                // 转向输出发给电机任务
                xQueueOverwrite(PID_Turn_Queue_MotoHandle, &TurnPID.Out);
            }

            // ---- 3.6 读取角度反馈（MPU） ----
            if(xQueueReceive(Queue_PIDHandle, &StartCoder_Center, 0) == pdTRUE)
            {
                AnglePID.Actual = StartCoder_Center.MPUData.Angle;
                PID_Update(&AnglePID);
                xQueueOverwrite(PID_Queue_MotorHandle, &AnglePID.Out);
            }
        }

        // 10ms 精确延时
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(10));
    }
}
