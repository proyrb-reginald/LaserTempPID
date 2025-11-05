#include <board.h>
#include <gpio.h>
#include <rtthread.h>

int main(void)
{
    while (1)
    {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_4);
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        rt_lprintf("usart6\n");
        rt_thread_mdelay(500);
    }
}