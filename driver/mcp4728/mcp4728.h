#ifndef MCP4728_H
#define MCP4728_H

/* 导入外部库 */
#define MCP4728_HAL_INC <stm32f4xx_hal.h>
#include MCP4728_HAL_INC
#define MCP4728_BSP_INC <board.h>
#include MCP4728_BSP_INC
#define MCP4728_RTOS_INC <rtthread.h>
#include MCP4728_RTOS_INC

/* 基本数据类型定义 */
#define MCP4728_UINT8_TYPE  uint8_t
#define MCP4728_INT16_TYPE  int16_t
#define MCP4728_UINT16_TYPE uint16_t

/* 串口日志 */
#define MCP4728_LOG 1
#if MCP4728_LOG == 1
/* 串口日志接口 */
#define MCP4728_LOG_INTERFACE rt_lprintf
#else
#define MCP4728_LOG_INTERFACE(...)
#endif

/* 提供延时函数 */
#define MCP4728_DELAY_INTERFACE rt_thread_mdelay

/* 接口实现 */
inline void mcp4728_i2c_transmit(MCP4728_UINT8_TYPE addr,
                                 MCP4728_UINT8_TYPE *data,
                                 MCP4728_UINT16_TYPE size)
{
    // #warning "Please implement the i2c transmit function!"
    HAL_I2C_Master_Transmit(&hi2c1, addr, data, size, 20);
}

inline void mcp4728_i2c_receive(MCP4728_UINT8_TYPE addr,
                                MCP4728_UINT8_TYPE *data,
                                MCP4728_UINT16_TYPE size)
{
    // #warning "Please implement the i2c receive function!"
    HAL_I2C_Master_Receive(&hi2c1, addr, data, size, 20);
}

inline void mcp4728_gpio_ldac_set(void)
{
    // #warning "Please implement the gpio ldac write function!"
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
}

inline void mcp4728_gpio_ldac_reset(void)
{
    // #warning "Please implement the gpio ldac write function!"
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
}

/* 导出接口 */
void mcp4728_init(void);
void mcp4728_set_dac_a(float rate);
void mcp4728_set_dac_b(float rate);

#endif /* MCP4728_H */