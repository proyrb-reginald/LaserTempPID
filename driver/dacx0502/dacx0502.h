#ifndef DACX0502_H
#define DACX0502_H

/* 导入外部库 */
#define DACX0502_HAL_INC <stm32f4xx_hal.h>
#include DACX0502_HAL_INC
#define DACX0502_BSP_INC <board.h>
#include DACX0502_BSP_INC
#define DACX0502_RTOS_INC <rtthread.h>
#include DACX0502_RTOS_INC

/* 基本数据类型定义 */
#define DACX0502_UINT8_TYPE  uint8_t
#define DACX0502_UINT16_TYPE uint16_t

/* 串口日志 */
#define DACX0502_LOG 1
#if DACX0502_LOG == 1
/* 串口日志接口 */
#define DACX0502_LOG_INTERFACE rt_lprintf
#else
#define DACX0502_LOG_INTERFACE(...)
#endif

/* 提供延时函数 */
#define DACX0502_DELAY_INTERFACE rt_thread_mdelay

/* 指定串行接口并实现发送与接收函数 */
#define DACX0502_SERIAL_INTERFACE 0
#if DACX0502_SERIAL_INTERFACE
inline void dacx0502_i2c_transmit(DACX0502_UINT8_TYPE *data,
                                  DACX0502_UINT16_TYPE size)
{
#warning "Please implement the i2c transmit function!"
}

inline void dacx0502_i2c_receive(DACX0502_UINT8_TYPE *data,
                                 DACX0502_UINT16_TYPE size)
{
#warning "Please implement the i2c receive function!"
}
#else
void dacx0502_spi_transmit(DACX0502_UINT8_TYPE *data, DACX0502_UINT16_TYPE size)
{
#warning "Please implement the spi transmit function!"
}
void dacx0502_spi_receive(DACX0502_UINT8_TYPE *data, DACX0502_UINT16_TYPE size)
{
#warning "Please implement the spi receive function!"
}
#endif

/* DAC80502填16 | DAC70502填14 | DAC60502填12 */
#define DACX0502_DEVICE 16

/* 使用外部参考电压填1 | 使用内部参考电压填0 */
#define DACX0502_USE_EXTERNAL_REF_VOLTAGE 0
#if DACX0502_USE_EXTERNAL_REF_VOLTAGE
#define DACX0502_REF_VOLTAGE 3.3f
#else
#define DACX0502_REF_VOLTAGE 2.5f
#endif

/* 导出接口 */
void dacx0502_init(void);
void dacx0502_set_dac_a(float rate);
void dacx0502_set_dac_b(float rate);

#endif /* DACX0502_H */