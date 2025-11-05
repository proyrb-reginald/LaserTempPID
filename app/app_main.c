#include <gpio.h>
#include <stm32f4xx_hal.h>
#include <usart.h>

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART1_UART_Init();
    MX_USART6_UART_Init();

    while (1)
    {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_4);
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        HAL_UART_Transmit(&huart1, (uint8_t *)"usart1\r\n", 8, HAL_MAX_DELAY);
        HAL_UART_Transmit(&huart6, (uint8_t *)"usart2\r\n", 8, HAL_MAX_DELAY);
        HAL_Delay(500);
    }
}