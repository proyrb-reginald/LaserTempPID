#include <ads1118.h>
#include <board.h>
#include <math.h>
#include <mcp4728.h>
#include <rtthread.h>
#ifndef APP_CMD_START
#include <app_cmd.h>
#include <app_ctr.h>
#endif

// y = 302.007946 * x^(-0.080455) + 14.022974
#define CURRENT (1.235f / 24.91f)

inline float exp_fit(float x)
{
    return 302.007946f * powf(x, -0.080455f) + 14.022974f;
}

void thread_test(void *parameter)
{
    mcp4728_init();
    ads1118_init();
    ads1118_set_channel(7);
    while (1)
    {
        /* Channel-A对应DAC_L Channel-B对应DAC_H */
        /* DAC_L高(低)TEC-高(低) DAC_H高(低)TEC+高(低) */
        // static uint8_t cnt = 0;
        // if (cnt++ % 2 == 0)
        // {
        //     mcp4728_set_dac_a(1.0f);
        //     mcp4728_set_dac_b(0.0f);
        // }
        // else
        // {
        //     mcp4728_set_dac_a(0.0f);
        //     mcp4728_set_dac_b(1.0f);
        // }
        mcp4728_set_dac_a(0.0f);
        mcp4728_set_dac_b(1.0f);

        float rate = ads1118_read_channel();
        float voltage = rate * 4.096f;

        // float omp = voltage / CURRENT;
        // float temp = exp_fit(omp);
        // rt_lprintf("rate:%d voltage:%d omp:%d temp:%d\n",
        //            (int16_t)(rate * 1000), (int16_t)(voltage * 1000),
        //            (int16_t)(omp * 1000), (int16_t)(temp * 1));

        rt_lprintf("rate:%d voltage:%d\n", (int16_t)(rate * 1000),
                   (int16_t)(voltage * 1000));

        rt_thread_mdelay(1000);
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