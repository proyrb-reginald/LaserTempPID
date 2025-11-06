#include <ads1118.h>
#include <board.h>
#include <mcp4728.h>
#include <rtthread.h>
#ifndef APP_CMD_START
#include <app_cmd.h>
#include <app_ctr.h>
#endif

void thread_test(void *parameter)
{
    mcp4728_init();
    ads1118_init();
    ads1118_set_channel(6);
    while (1)
    {
        static uint8_t cnt = 0;
        if (cnt++ % 2 == 0)
        {
            mcp4728_set_dac_a(1.0f);
            mcp4728_set_dac_b(0.0f);
            // rt_lprintf("set a reset b\n");
        }
        else
        {
            mcp4728_set_dac_a(0.0f);
            mcp4728_set_dac_b(1.0f);
            // rt_lprintf("set b reset a\n");
        }
        float rate = ads1118_read_channel();
        rt_lprintf("rate:%d\n", (int16_t)(rate * 10000));
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