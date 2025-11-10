#ifndef ADS1118_H
#define ADS1118_H

/* 导入外部库 */
#define ADS1118_HAL_INC <stm32f4xx_hal.h>
#include ADS1118_HAL_INC
#define ADS1118_BSP_INC <board.h>
#include ADS1118_BSP_INC
#define ADS1118_RTOS_INC <rtthread.h>
#include ADS1118_RTOS_INC

/* 定义数据类型接口 */
#define ADS1118_UINT8_TYPE  uint8_t
#define ADS1118_UINT16_TYPE uint16_t

/* 配置串口日志 */
#define ADS1118_LOG 0
#if ADS1118_LOG == 1
/* 串口日志接口 */
#define ADS1118_LOG_INTERFACE rt_lprintf
#else
#define ADS1118_LOG_INTERFACE(...)
#endif

#define ADS1118_VOLTAGE_VREF 2.048f

/* 提供延时函数 */
#define ADS1118_DELAY_INTERFACE rt_thread_mdelay

/* 接口实现 */
inline void ads1118_spi_transmit(ADS1118_UINT8_TYPE *data,
                                 ADS1118_UINT16_TYPE size)
{
    // #warning "Please implement the spi transmit function!"
    HAL_SPI_Transmit(&hspi3, data, size, 10);
}

inline void ads1118_spi_receive(ADS1118_UINT8_TYPE *data,
                                ADS1118_UINT16_TYPE size)
{
    // #warning "Please implement the spi receive function!"
    HAL_SPI_Receive(&hspi3, data, size, 10);
}

inline void ads1118_gpio_cs_set(void)
{
    // #warning "Please implement the gpio cs write function!"
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
}

inline void ads1118_gpio_cs_reset(void)
{
    // #warning "Please implement the gpio cs write function!"
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
}

/* 导出接口 */
void ads1118_init(void);

/**
 * channel 选择采样通道
 * 0: AIN0-AIN1
 * 1: AIN0-AIN3
 * 2: AIN1-AIN3
 * 3: AIN2-AIN3
 * 4: AIN0-GND
 * 5: AIN1-GND
 * 6: AIN2-GND
 * 7: AIN3-GND
 */
void ads1118_set_channel(ADS1118_UINT8_TYPE channel);

float ads1118_read_channel(void);

#endif /* ADS1118_H */