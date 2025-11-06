#include <board.h>
#include <mcp4728.h>
#include <rtthread.h>

int main(void)
{
    mcp4728_init();
    while (1)
    {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_4);
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        static uint8_t cnt = 0;
        if (cnt++ % 2 == 0)
        {
            rt_lprintf("0.0f\n");
            mcp4728_set_dac_a(0.0f);
            mcp4728_set_dac_b(0.0f);
        }
        else
        {
            rt_lprintf("1.0f\n");
            mcp4728_set_dac_a(1.0f);
            mcp4728_set_dac_b(1.0f);
        }
        rt_thread_mdelay(1000);
    }
}