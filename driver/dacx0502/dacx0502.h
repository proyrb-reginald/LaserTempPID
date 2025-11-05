#ifndef DACX0502_H
#define DACX0502_H

#include <stm32f4xx_hal.h>

/* 串口调试 */
#define DACX0502_DEBUG 0

/* 基本数据类型定义 */
#define DACX0502_UINT8_TYPE  uint8_t
#define DACX0502_UINT16_TYPE uint16_t

/* 指定串行接口并实现发送与接收函数 */
#define DACX0502_SERIAL_INTERFACE 0
#if DACX0502_SERIAL_INTERFACE
#include <i2c.h>
#ifdef DACX0502_C
inline void dacx0502_i2c_transmit(DACX0502_UINT8_TYPE *data,
                                  DACX0502_UINT16_TYPE size)
{
#error "Please implement the i2c transmit function!"
}

inline void dacx0502_i2c_receive(DACX0502_UINT8_TYPE *data,
                                 DACX0502_UINT16_TYPE size)
{
#error "Please implement the i2c receive function!"
}
#endif
#else
#ifdef DACX0502_C
#include <spi.h>
void dacx0502_spi_transmit(DACX0502_UINT8_TYPE *data, DACX0502_UINT16_TYPE size)
{
    // #error "Please implement the spi transmit function!"
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi2, data, size, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
}
void dacx0502_spi_receive(DACX0502_UINT8_TYPE *data, DACX0502_UINT16_TYPE size)
{
    // #error "Please implement the spi receive function!"
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
    HAL_SPI_Receive(&hspi2, data, size, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
}
#endif
#endif

/* 提供延时函数 */
#include <rtthread.h>
#define DACX0502_DELAY_INTERFACE rt_thread_delay

/* 提供串口调试函数 */
#define DACX0502_DEBUG_INTERFACE rt_kprintf

/* DAC80502填16 | DAC70502填14 | DAC60502填12 */
#define DACX0502_DEVICE 16

/* 使用外部参考电压填1 | 使用内部参考电压填0 */
#define DACX0502_USE_EXTERNAL_REF_VOLTAGE 0
#if DACX0502_USE_EXTERNAL_REF_VOLTAGE
#define DACX0502_REF_VOLTAGE 3.3f
#else
#define DACX0502_REF_VOLTAGE 2.5f
#endif

/* 导出函数 */
void dacx0502_init(void);
void dacx0502_set_dac_a(float rate);
void dacx0502_set_dac_b(float rate);

#endif /* DACX0502_H */