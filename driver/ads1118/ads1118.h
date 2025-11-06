#ifndef ADS1118_H
#define ADS1118_H

/* 提供HAL库 */
#include <stm32f4xx_hal.h>
#include <board.h>

/* 基本数据类型定义 */
#define ADS1118_UINT8_TYPE  uint8_t
#define ADS1118_UINT16_TYPE uint16_t

/* 串口日志 */
#define ADS1118_DEBUG 1
#if ADS1118_DEBUG == 1
/* 提供串口日志输出函数 */
#define ADS1118_DEBUG_INTERFACE rt_lprintf
#else
#define ADS1118_DEBUG_INTERFACE(...)
#endif

#ifdef ADS1118_C
/* 指定串行接口并实现发送与接收函数 */
inline void ads1118_spi_transmit(ADS1118_UINT8_TYPE *data,
                                 ADS1118_UINT16_TYPE size)
{
    // #error "Please implement the spi transmit function!"
    HAL_SPI_Transmit(&hspi3, data, size, 20);
}

inline void ads1118_spi_receive(ADS1118_UINT8_TYPE *data,
                                ADS1118_UINT16_TYPE size)
{
    // #error "Please implement the spi receive function!"
    HAL_SPI_Receive(&hspi3, data, size, 20);
}

inline void ads1118_gpio_cs_set(void)
{
    // #error "Please implement the gpio cs write function!"
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
}

inline void ads1118_gpio_cs_reset(void)
{
    // #error "Please implement the gpio cs write function!"
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
}
#endif

/* 提供延时函数 */
#include <rtthread.h>
#define ADS1118_DELAY_INTERFACE rt_thread_mdelay

/* 使用外部参考电压填1 | 使用内部参考电压填0 */
#define ADS1118_USE_EXTERNAL_REF_VOLTAGE 0
#if ADS1118_USE_EXTERNAL_REF_VOLTAGE
#define ADS1118_REF_VOLTAGE 3.3f
#else
#define ADS1118_REF_VOLTAGE 2.048f
#endif

/* 导出函数 */
void ads1118_init(void);
void ads1118_read_ain(void);

#endif /* ADS1118_H */