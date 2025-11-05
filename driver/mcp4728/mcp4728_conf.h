#ifndef MCP4728_CONF_H
#define MCP4728_CONF_H

#include "main.h"

/* --- 用户配置区 --- */

/* I2C Handle: 指定用于与MCP4728通信的I2C实例 */
extern I2C_HandleTypeDef hi2c1; /* 请根据实际情况修改 */
#define MCP4728_I2C_HANDLE hi2c1

/* I2C地址: 根据硬件连接设置MCP4728的I2C地址 */
#define MCP4728_I2C_ADDR MCP4728_BASE_ADDR /* 默认A0引脚接地 */

/* 超时设置: I2C通信超时时间 (单位: ms) */
#define MCP4728_TIMEOUT_MS 20

/* --- 配置校验 --- */

/* 检查是否定义了I2C句柄 */
#ifndef MCP4728_I2C_HANDLE
#error "MCP4728: Please define MCP4728_I2C_HANDLE in mcp4728_conf.h"
#endif

#endif /* MCP4728_CONF_H */