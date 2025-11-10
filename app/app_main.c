#include <ads1118.h>
#include <board.h>
#include <errno.h>
#include <math.h>
#include <mcp4728.h>
#include <rtthread.h>
#include <stdlib.h>
#include <string.h>

#ifndef APP_CMD_START
#include <app_cmd.h>
#include <app_ctr.h>
#endif

// 电流计算常量 (1.235V / 24.91K ohm)
#define CURRENT_CONST (1.235f / 24.91f)

// DAC输出电压转换
#define SET_VOLTAGE(x) (x / MCP4728_REF_VOLTAGE)

/**
 * @brief 指数拟合函数，用于将传感器电阻转换为温度
 * @param x 传感器电阻值(ohm)
 * @return 计算得到的温度值(Kelvin)
 */
inline float exp_fit(float x)
{
    // 电阻-温度指数拟合函数参数 y = 264.549764 * x^(-0.098781) + 52.938906
    return 264.549764f * powf(x, -0.098781f) + 52.938906f;
}

/* 温度PID控制参数定义 */
#define MAX_CURRENT 1.2f                                   // 最大电流限制 (A)
#define MIN_CURRENT (-MAX_CURRENT)                         // 最小电流 (-1.2A)
#define TEMP_PID_TARGET 22.0f                              // 目标温度值(摄氏度)
#define TEMP_PID_KP 0.3f                                   // 温度PID比例系数
#define TEMP_PID_KI 0.01f                                  // 温度PID积分系数
#define TEMP_PID_KD 0.0f                                   // 温度PID微分系数
#define TEMP_PID_INTEGRAL_MAX (MAX_CURRENT / TEMP_PID_KI)  // 温度PID积分上限
#define TEMP_PID_INTEGRAL_MIN (-MAX_CURRENT / TEMP_PID_KI) // 温度PID积分下限

/* 全局变量声明 */
static float temp_pid_target = TEMP_PID_TARGET; // 目标温度
static float temp_pid_kp = TEMP_PID_KP;         // PID比例系数
static float temp_pid_ki = TEMP_PID_KI;         // PID积分系数
static float temp_pid_kd = TEMP_PID_KD;         // PID微分系数
static uint8_t mode = 0;                        // 工作模式: 0-制冷, 1-加热

/**
 * @brief 设置PID参数和目标值的命令处理函数
 * @param argc 参数个数
 * @param argv 参数数组
 * @return int 执行结果
 */
int msh_set_value(int argc, char **argv)
{
    // 检查参数数量
    if (argc < 3)
    {
        rt_kprintf("argc is not enough\n");
        return 0;
    }

    // 解析数值参数
    float data = 0.0f;
    {
        char *endptr;
        errno = 0;
        data = strtof(argv[2], &endptr);
        if (endptr == argv[2])
        {
            rt_kprintf("invalid argv\n");
            return 0;
        }
        else if (errno == ERANGE)
        {
            rt_kprintf("overflow\n");
            return 0;
        }
    }

    // 根据第一个参数设置对应的变量
    if (strcmp("t", argv[1]) == 0)
    {
        temp_pid_target = data;
    }
    else if (strcmp("tp", argv[1]) == 0)
    {
        temp_pid_kp = data;
    }
    else if (strcmp("ti", argv[1]) == 0)
    {
        temp_pid_ki = data;
    }
    else if (strcmp("td", argv[1]) == 0)
    {
        temp_pid_kd = data;
    }
    else if (strcmp("m", argv[1]) == 0)
    {
        mode = (uint8_t)data;
    }
    else
    {
        rt_kprintf("unkown %s\n", argv[1]);
        return 0;
    }

    rt_kprintf("set finish\n");
    return 0;
}
MSH_CMD_EXPORT_ALIAS(msh_set_value, set_value, set t - p - i - d - m);

/**
 * @brief 温度控制线程函数
 * @param parameter 线程参数
 */
void thread_test(void *parameter)
{
    // 初始化外设
    mcp4728_init();
    ads1118_init();

    /* 温度PID控制变量初始化 */
    float temp_pid_last_error = 0.0f; // 温度PID上一次误差
    float temp_pid_integral = 0.0f;   // 温度PID积分项

    while (1)
    {
        /* 读取实际电流转电压ADC值 */
        ads1118_set_channel(7);
        rt_thread_mdelay(3);
        float voltage = ads1118_read_channel() * 4.096f; // 转换为实际电压值

        /* 读取传感器的温度转电压值 */
        ads1118_set_channel(6);
        rt_thread_mdelay(3);
        float current_temp =
            exp_fit(ads1118_read_channel() * 4.096f / CURRENT_CONST) -
            273.15f; // 转换为摄氏度

        /* 温度PID控制器计算 */
        float temp_pid_error = temp_pid_target - current_temp; // 当前温度误差
        temp_pid_integral += temp_pid_error;                   // 积分累加

        /* 温度PID积分限幅 */
        if (temp_pid_integral > TEMP_PID_INTEGRAL_MAX)
        {
            temp_pid_integral = TEMP_PID_INTEGRAL_MAX;
        }
        else if (temp_pid_integral < TEMP_PID_INTEGRAL_MIN)
        {
            temp_pid_integral = TEMP_PID_INTEGRAL_MIN;
        }

        // 计算微分项和PID输出
        float temp_pid_derivative =
            temp_pid_error - temp_pid_last_error; // 微分项
        float temp_pid_output = temp_pid_kp * temp_pid_error +
                                temp_pid_ki * temp_pid_integral +
                                temp_pid_kd * temp_pid_derivative; // PID输出

        /* 限制温度PID输出范围 */
        if (temp_pid_output > MAX_CURRENT)
        {
            temp_pid_output = MAX_CURRENT;
        }
        else if (temp_pid_output < MIN_CURRENT)
        {
            temp_pid_output = MIN_CURRENT;
        }

        /* 根据温度PID输出确定工作模式 */
        if (temp_pid_output >= 0)
        {
            mode = 1; // 加热模式
        }
        else
        {
            mode = 0; // 制冷模式
        }

        /* 设置DAC输出 */
        float abs_output = fabsf(temp_pid_output); // 获取绝对值
        if (mode == 0)
        {
            // 制冷模式
            mcp4728_set_dac_a(SET_VOLTAGE(0.0f));
            mcp4728_set_dac_b(SET_VOLTAGE(abs_output));
        }
        else
        {
            // 加热模式
            mcp4728_set_dac_a(SET_VOLTAGE(abs_output));
            mcp4728_set_dac_b(SET_VOLTAGE(0.0f));
        }

        temp_pid_last_error = temp_pid_error; // 更新温度PID上一次误差

        /* 打印调试信息（单位mV, mA, °C）*/
        rt_lprintf("%d,%d,%d,%d\n", (int32_t)(temp_pid_target * 1000),
                   (int32_t)(temp_pid_output * 1000), (int32_t)(voltage * 1000),
                   (int32_t)(current_temp * 1000));

        // 控制周期延时
        rt_thread_mdelay(1);
    }
}

/**
 * @brief 主函数
 *
 * @return int 返回值
 */
int main(void)
{
#ifndef APP_CMD_START
    char *start_app_ctr_argv[] = {"start_app", "ctr"};
    msh_start_app(2, start_app_ctr_argv);
#endif

    // 创建并启动温度控制线程
    rt_thread_t ctr_tid =
        rt_thread_create("test", thread_test, RT_NULL, 512, 3, 0);
    if (ctr_tid != RT_NULL)
    {
        if (rt_thread_startup(ctr_tid) == RT_EOK)
        {
            rt_kprintf("thread[%s] start finish.\n", ctr_tid->name);
        }
        else
        {
            rt_kprintf("thread[%s] start fail.\n", ctr_tid->name);
            rt_thread_delete(ctr_tid);
        }
    }
    else
    {
        rt_kprintf("thread create fail\n");
    }

    // 主循环 - LED指示灯闪烁
    while (1)
    {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_4);
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        rt_thread_mdelay(500);
    }
}