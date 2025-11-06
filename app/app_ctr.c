#include <app_ctr.h>
#include <dacx0502.h>

#define SET_CURRENT(x) ((float)x / 100) / DACX0502_REF_VOLTAGE
#define SET_TEMPERATURE(x) ((float)x - 7) / 40 * 3.3f / 2 / DACX0502_REF_VOLTAGE

void thread_ctr(void *parameter)
{
    /* 初始化DACX0502 */
    dacx0502_init();

    /* 开启温控板与激光器驱动板使能 */
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_SET);

    rt_tick_t next_tick = rt_tick_get();
    while (1)
    {
        static uint8_t current_cnt = 0;
        static uint8_t temperature_cnt = 0;
        if (parameter == RT_NULL)
        {
            rt_kprintf("app_ctr: parameter is null.\n");
            rt_thread_delete(rt_thread_self());
        }
        app_ctr_args *args = (app_ctr_args *)parameter;

        if (args->current_mode)
        {
            /* 设置激光器驱动跳变电流 */
            if (current_cnt++ % 2 == 0)
            {
                dacx0502_set_dac_a(SET_CURRENT(args->current));
            }
            else
            {
                dacx0502_set_dac_a(SET_CURRENT(args->current) / 2);
            }
        }
        else
        {
            /* 设置激光器驱动恒定电流 */
            dacx0502_set_dac_a(SET_CURRENT(args->current));
        }

        if (args->temperature_mode)
        {
            /* 设置温控板控制跳变温度 */
            if (temperature_cnt++ % 2 == 0)
            {
                dacx0502_set_dac_b(SET_TEMPERATURE(args->temperature));
            }
            else
            {
                dacx0502_set_dac_b(SET_TEMPERATURE(args->temperature - 4));
            }
            /* 固定周期调用 */
        }
        else
        {
            /* 设置温控板控制恒定温度 */
            dacx0502_set_dac_b(SET_TEMPERATURE(args->temperature));
        }

        rt_thread_delay_until(&next_tick, 5000);
    }
}
