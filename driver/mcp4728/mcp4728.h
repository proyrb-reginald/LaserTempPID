#ifndef MCP4728_H
#define MCP4728_H

/* 提供HAL库 */
#include <stm32f4xx_hal.h>
#include <board.h>

/* 基本数据类型定义 */
#define MCP4728_UINT8_TYPE  uint8_t
#define MCP4728_INT16_TYPE  int16_t
#define MCP4728_UINT16_TYPE uint16_t

/* 串口日志 */
#define MCP4728_DEBUG 1
#if MCP4728_DEBUG == 1
/* 提供串口日志输出函数 */
#define MCP4728_DEBUG_INTERFACE rt_lprintf
#else
#define MCP4728_DEBUG_INTERFACE(...)
#endif

#ifdef MCP4728_C
/* 指定串行接口并实现发送与接收函数 */
inline void mcp4728_i2c_transmit(MCP4728_UINT8_TYPE addr,
                                 MCP4728_UINT8_TYPE *data,
                                 MCP4728_UINT16_TYPE size)
{
    // #error "Please implement the i2c transmit function!"
    HAL_I2C_Master_Transmit(&hi2c1, addr, data, size, 20);
}

inline void mcp4728_i2c_receive(MCP4728_UINT8_TYPE addr,
                                MCP4728_UINT8_TYPE *data,
                                MCP4728_UINT16_TYPE size)
{
    // #error "Please implement the i2c receive function!"
    HAL_I2C_Master_Receive(&hi2c1, addr, data, size, 20);
}

inline void mcp4728_gpio_ldac_set(void)
{
    // #error "Please implement the gpio ldac write function!"
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
}

inline void mcp4728_gpio_ldac_reset(void)
{
    // #error "Please implement the gpio ldac write function!"
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
}
#endif

/* 提供延时函数 */
#include <rtthread.h>
#define MCP4728_DELAY_INTERFACE rt_thread_mdelay

/* 使用外部参考电压填1 | 使用内部参考电压填0 */
#define MCP4728_USE_EXTERNAL_REF_VOLTAGE 0
#if MCP4728_USE_EXTERNAL_REF_VOLTAGE
#define MCP4728_REF_VOLTAGE 3.3f
#else
#define MCP4728_REF_VOLTAGE 2.048f
#endif

/* 导出函数 */
void mcp4728_init(void);
void mcp4728_set_dac_a(float rate);
void mcp4728_set_dac_b(float rate);

#endif /* MCP4728_H */