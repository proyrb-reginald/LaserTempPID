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

// y = 264.549764 * x^(-0.098781) + 52.938906

#define CURRENT (1.235f / 24.91f) // 1.235V / 24.91K

/* 指数拟合函数 */
inline float exp_fit(float x)
{
    return 264.549764f * powf(x, -0.098781f) + 52.938906f;
}

/* PID控制参数定义 */
#define PID_TARGET_VOLTAGE 0.0f            /* 目标电压值 */
#define PID_KP 0.001f                      /* 比例系数 */
#define PID_KI 0.3f                        /* 积分系数 */
#define PID_KD 0.0f                        /* 微分系数 */
#define PID_INTEGRAL_MAX (2.048f / pid_ki) /* 积分上限 */
#define PID_INTEGRAL_MIN 0.0f              /* 积分下限 */
#define PID_OUTPUT_MAX 2.048f              /* 输出上限 */
#define PID_OUTPUT_MIN 0.0f                /* 输出下限 */

#define SET_VOLTAGE(x) (x / MCP4728_REF_VOLTAGE)

static float pid_target = PID_TARGET_VOLTAGE; /* 目标电压 */
static float pid_kp = PID_KP;
static float pid_ki = PID_KI;
static float pid_kd = PID_KD;
static uint8_t mode = 0;

int msh_set_value(int argc, char **argv)
{
    float data = 0.0f;
    {
        char *endptr;
        errno = 0;
        data = strtof(argv[2], &endptr);
        if (endptr == argv[2])
        {
            rt_kprintf("invalid input\n");
            return 0;
        }
        else if (errno == ERANGE)
        {
            rt_kprintf("over range of float\n");
            return 0;
        }
    }

    if (strcmp("p", argv[1]) == 0)
    {
        pid_kp = data;
    }
    else if (strcmp("i", argv[1]) == 0)
    {
        pid_ki = data;
    }
    else if (strcmp("d", argv[1]) == 0)
    {
        pid_kd = data;
    }
    else if (strcmp("v", argv[1]) == 0)
    {
        pid_target = data;
    }
    else if (strcmp("m", argv[1]) == 0)
    {
        mode = data;
    }
    else
    {
        rt_kprintf("Usage: set_value [p/i/d/v/m] [value]\n");
        return 0;
    }
    rt_kprintf("set finish\n");
    return 0;
}
MSH_CMD_EXPORT_ALIAS(msh_set_value, set_value,
                     Usage : set_value[p / i / d / v / m][value]);

void thread_test(void *parameter)
{
    mcp4728_init();
    ads1118_init();

    /* PID控制变量初始化 */
    float pid_last_error = 0.0f; /* 上一次误差 */
    float pid_integral = 0.0f;   /* 积分项 */

    while (1)
    {
        /* 读取ADC值并转换为电压 */
        ads1118_set_channel(7);
        rt_thread_mdelay(3);
        float rate = ads1118_read_channel();
        float voltage = rate * 4.096f;

        /* PID控制器计算 */
        float pid_error = pid_target - voltage; /* 当前误差 */
        pid_integral += pid_error;              /* 积分累加 */

        /* 积分限幅 */
        if (pid_integral > PID_INTEGRAL_MAX)
            pid_integral = PID_INTEGRAL_MAX;
        else if (pid_integral < PID_INTEGRAL_MIN)
            pid_integral = PID_INTEGRAL_MIN;

        float pid_derivative = pid_error - pid_last_error; /* 微分项 */
        float pid_output = pid_kp * pid_error + pid_ki * pid_integral +
                           pid_kd * pid_derivative; /* PID输出 */

        /* 输出限幅 */
        if (pid_output > PID_OUTPUT_MAX)
            pid_output = PID_OUTPUT_MAX;
        else if (pid_output < PID_OUTPUT_MIN)
            pid_output = PID_OUTPUT_MIN;

        /* 设置DAC输出：DAC_A由PID控制，DAC_B保持1.2V */
        if (mode == 0) // cold
        {
            mcp4728_set_dac_a(SET_VOLTAGE(0.0f));
            mcp4728_set_dac_b(SET_VOLTAGE(pid_output)); // 1200 : 300mV
        }
        else
        {
            mcp4728_set_dac_a(SET_VOLTAGE(pid_output));
            mcp4728_set_dac_b(SET_VOLTAGE(0.0f));
        }

        pid_last_error = pid_error; /* 更新上一次误差 */

        /* 打印调试信息（单位mV） */
        ads1118_set_channel(6);
        rt_thread_mdelay(3);
        float adc_sensor = ads1118_read_channel();
        float sensor_v = adc_sensor * 4.096f;
        float sensor_o = sensor_v / CURRENT;
        float temp = exp_fit(sensor_o) - 273.15f;
        rt_lprintf("%d,%d,%d,%d,%d\n", (int32_t)(pid_target * 1000),
                   (int32_t)(pid_output * 1000), (int32_t)(voltage * 1000),
                   (int32_t)(sensor_o * 100), (int32_t)(temp * 1000));
    }
}

int main(void)
{
#ifndef APP_CMD_START
    char *start_app_ctr_argv[] = {"start_app", "ctr"};
    msh_start_app(2, start_app_ctr_argv);
#endif
    {
        rt_thread_t ctr_tid =
            rt_thread_create("test", thread_test, RT_NULL, 512, 3, 0);
        if (ctr_tid != RT_NULL)
        {
            if (rt_thread_startup(ctr_tid) == RT_EOK)
            {
                rt_kprintf("Thread[%s] started.\n", ctr_tid->name);
            }
            else
            {
                rt_kprintf("Failed to start thread[%s].\n", ctr_tid->name);
                rt_thread_delete(ctr_tid);
            }
        }
        else
        {
            rt_kprintf("Failed to create thread[%s].\n", ctr_tid->name);
        }
    }

    while (1)
    {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_4);
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        rt_thread_mdelay(500);
    }
}