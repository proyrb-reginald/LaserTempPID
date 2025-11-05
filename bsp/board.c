#include <board.h>
#include <rthw.h>
#include <rtthread.h>

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
RT_WEAK void *rt_heap_begin_get(void)
{
    extern char _heap_start;
    return &_heap_start;
}

RT_WEAK void *rt_heap_end_get(void)
{
    extern char _estack;
    return &_estack;
}
#endif

void rt_os_tick_callback(void)
{
    rt_interrupt_enter();
    rt_tick_increase();
    rt_interrupt_leave();
}

void rt_hw_board_init(void)
{
    HAL_Init();
    SystemClock_Config();
    SystemCoreClockUpdate();
    HAL_SYSTICK_Config(HAL_RCC_GetSysClockFreq() / RT_TICK_PER_SECOND);
    MX_GPIO_Init();
    MX_I2C1_Init();

#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
#endif
}

#ifdef RT_USING_CONSOLE
static int uart_init(void)
{
    MX_USART1_UART_Init();
    MX_USART6_UART_Init();
    return 0;
}
INIT_BOARD_EXPORT(uart_init);

void rt_hw_console_output(const char *str)
{
    rt_size_t i = 0, size = 0;
    char a = '\r';

    __HAL_UNLOCK(&huart1);

    size = rt_strlen(str);
    for (i = 0; i < size; i++)
    {
        if (*(str + i) == '\n')
        {
            HAL_UART_Transmit(&huart1, (uint8_t *)&a, 1, 1);
        }
        HAL_UART_Transmit(&huart1, (uint8_t *)(str + i), 1, 1);
    }
}

void rt_hw_log_output(const char *str)
{
    rt_size_t i = 0, size = 0;
    char a = '\r';

    __HAL_UNLOCK(&huart6);

    size = rt_strlen(str);
    for (i = 0; i < size; i++)
    {
        if (*(str + i) == '\n')
        {
            HAL_UART_Transmit(&huart6, (uint8_t *)&a, 1, 1);
        }
        HAL_UART_Transmit(&huart6, (uint8_t *)(str + i), 1, 1);
    }
}

int rt_lprintf(const char *fmt, ...)
{
    va_list args;
    rt_size_t length;
    static char rt_log_buf[RT_CONSOLEBUF_SIZE];

    va_start(args, fmt);
    length = rt_vsnprintf(rt_log_buf, sizeof(rt_log_buf) - 1, fmt, args);
    if (length > RT_CONSOLEBUF_SIZE - 1)
        length = RT_CONSOLEBUF_SIZE - 1;
#ifdef RT_USING_DEVICE
    if (_console_device == RT_NULL)
    {
        rt_hw_log_output(rt_log_buf);
    }
    else
    {
        rt_device_write(_console_device, 0, rt_log_buf, length);
    }
#else
    rt_hw_log_output(rt_log_buf);
#endif /* RT_USING_DEVICE */
    va_end(args);

    return length;
}
RTM_EXPORT(rt_lprintf);
#endif

#ifdef RT_USING_FINSH
char rt_hw_console_getchar(void)
{
    int ch = -1;
    if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE) != RESET)
    {
        ch = huart1.Instance->DR & 0xFF;
    }
    else
    {
        if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_ORE) != RESET)
        {
            __HAL_UART_CLEAR_OREFLAG(&huart1);
        }
        rt_thread_mdelay(10);
    }
    return ch;
}
#endif