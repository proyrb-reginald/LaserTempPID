#ifndef MCP4728_H
#define MCP4728_H

#include "mcp4728_conf.h"

/* MCP4728 I2C地址 */
#define MCP4728_BASE_ADDR    0x60 /* 基础地址 A2=A1=0 */
#define MCP4728_ADDR_A0_HIGH 0x61 /* A0引脚为高时的地址 */

/* MCP4728命令类型 */
#define MCP4728_CMD_FAST_WRITE   0x00 /* 快速写命令 */
#define MCP4728_CMD_WRITE_REG    0x40 /* 写寄存器命令 */
#define MCP4728_CMD_WRITE_EEPROM 0x60 /* 写EEPROM命令 */
#define MCP4728_CMD_READ_REG     0x90 /* 读寄存器命令 */

/* MCP4728通道定义 */
#define MCP4728_CHANNEL_A 0x00
#define MCP4728_CHANNEL_B 0x01
#define MCP4728_CHANNEL_C 0x02
#define MCP4728_CHANNEL_D 0x03

/* MCP4728电源模式 */
#define MCP4728_PWR_MODE_NORMAL  0x00 /* 正常模式 */
#define MCP4728_PWR_MODE_PD_1K   0x01 /* 1K欧姆下拉模式 */
#define MCP4728_PWR_MODE_PD_100K 0x02 /* 100K欧姆下拉模式 */
#define MCP4728_PWR_MODE_PD_500K 0x03 /* 500K欧姆下拉模式 */

/* MCP4728参考电压选择 */
#define MCP4728_REF_MODE_VDD      0x00 /* 使用VDD作为参考电压 */
#define MCP4728_REF_MODE_INTERNAL 0x01 /* 使用内部2.048V参考电压 */

/* MCP4728增益选择 */
#define MCP4728_GAIN_X1 0x00 /* 增益为1 */
#define MCP4728_GAIN_X2 0x01 /* 增益为2 */

/**
 * @brief MCP4728通道配置结构体
 */
typedef struct {
    uint16_t data;      /* DAC数据值 (12位) */
    uint8_t power_mode; /* 电源模式 */
    uint8_t ref_mode;   /* 参考电压模式 */
    uint8_t gain;       /* 增益设置 */
} mcp4728_channel_config_t;

/**
 * @brief 初始化MCP4728模块
 * @retval 0: 成功, 其他: 失败
 */
uint8_t mcp4728_init(void);

/**
 * @brief 快速写入单个通道
 * @param channel 通道号 (MCP4728_CHANNEL_A-D)
 * @param data DAC数据值 (12位)
 * @retval 0: 成功, 其他: 失败
 */
uint8_t mcp4728_fast_write(uint8_t channel, uint16_t data);

/**
 * @brief 写入单个通道配置到寄存器
 * @param channel 通道号 (MCP4728_CHANNEL_A-D)
 * @param config 通道配置结构体指针
 * @retval 0: 成功, 其他: 失败
 */
uint8_t mcp4728_write_channel_reg(uint8_t channel,
                                  const mcp4728_channel_config_t *config);

/**
 * @brief 写入单个通道配置到寄存器和EEPROM
 * @param channel 通道号 (MCP4728_CHANNEL_A-D)
 * @param config 通道配置结构体指针
 * @retval 0: 成功, 其他: 失败
 */
uint8_t mcp4728_write_channel_eeprom(uint8_t channel,
                                     const mcp4728_channel_config_t *config);

/**
 * @brief 同时写入所有通道配置到寄存器
 * @param configs 四个通道配置结构体数组指针
 * @retval 0: 成功, 其他: 失败
 */
uint8_t mcp4728_write_all_channels_reg(const mcp4728_channel_config_t *configs);

/**
 * @brief 同时写入所有通道配置到寄存器和EEPROM
 * @param configs 四个通道配置结构体数组指针
 * @retval 0: 成功, 其他: 失败
 */
uint8_t mcp4728_write_all_channels_eeprom(
    const mcp4728_channel_config_t *configs);

/**
 * @brief 读取所有通道的寄存器配置
 * @param configs 四个通道配置结构体数组指针
 * @retval 0: 成功, 其他: 失败
 */
uint8_t mcp4728_read_all_channels(mcp4728_channel_config_t *configs);

#endif /* MCP4728_H */