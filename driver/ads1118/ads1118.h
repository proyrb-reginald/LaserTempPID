#ifndef ADS1118_H
#define ADS1118_H

#include "ads1118_conf.h"

/* --- 配置寄存器位定义 (Bit 15-0) --- */

/* Bit 15: SS (触发单次转换) */
#define ADS1118_SS_BIT        15
#define ADS1118_SS_NO_EFFECT  (0 << ADS1118_SS_BIT)
#define ADS1118_SS_START_CONV (1 << ADS1118_SS_BIT)

/* Bits 14-12: MUX (差分信号输入源选择) */
#define ADS1118_MUX_BIT       12
#define ADS1118_MUX_AIN0_AIN1 (0x0 << ADS1118_MUX_BIT) /* AIN0, AIN1 (默认) */
#define ADS1118_MUX_AIN0_AIN3 (0x1 << ADS1118_MUX_BIT) /* AIN0, AIN3 */
#define ADS1118_MUX_AIN1_AIN3 (0x2 << ADS1118_MUX_BIT) /* AIN1, AIN3 */
#define ADS1118_MUX_AIN2_AIN3 (0x3 << ADS1118_MUX_BIT) /* AIN2, AIN3 */
#define ADS1118_MUX_AIN0_GND  (0x4 << ADS1118_MUX_BIT) /* AIN0, AIN1=GND */
#define ADS1118_MUX_AIN1_GND  (0x5 << ADS1118_MUX_BIT) /* AIN1, AIN0=GND */
#define ADS1118_MUX_AIN2_GND  (0x6 << ADS1118_MUX_BIT) /* AIN2, AIN0=GND */
#define ADS1118_MUX_AIN3_GND  (0x7 << ADS1118_MUX_BIT) /* AIN3, AIN0=GND */

/* Bits 11-9: PGA (量程范围选择) */
#define ADS1118_PGA_BIT    9
#define ADS1118_PGA_6_144V (0x0 << ADS1118_PGA_BIT) /* FSR = ±6.144V */
#define ADS1118_PGA_4_096V (0x1 << ADS1118_PGA_BIT) /* FSR = ±4.096V */
#define ADS1118_PGA_2_048V (0x2 << ADS1118_PGA_BIT) /* FSR = ±2.048V (默认) */
#define ADS1118_PGA_1_024V (0x3 << ADS1118_PGA_BIT) /* FSR = ±1.024V */
#define ADS1118_PGA_0_512V (0x4 << ADS1118_PGA_BIT) /* FSR = ±0.512V */
#define ADS1118_PGA_0_256V (0x5 << ADS1118_PGA_BIT) /* FSR = ±0.256V */

/* Bit 8: MODE (连续转换或单次转换模式) */
#define ADS1118_MODE_BIT    8
#define ADS1118_MODE_CONT   (0 << ADS1118_MODE_BIT) /* 连续转换模式 */
#define ADS1118_MODE_SINGLE (1 << ADS1118_MODE_BIT) /* 单次转换模式 (默认) */

/* Bits 7-5: DR (数据采集率选择) */
#define ADS1118_DR_BIT    5
#define ADS1118_DR_8SPS   (0x0 << ADS1118_DR_BIT) /* 8 SPS */
#define ADS1118_DR_16SPS  (0x1 << ADS1118_DR_BIT) /* 16 SPS */
#define ADS1118_DR_32SPS  (0x2 << ADS1118_DR_BIT) /* 32 SPS */
#define ADS1118_DR_64SPS  (0x3 << ADS1118_DR_BIT) /* 64 SPS */
#define ADS1118_DR_128SPS (0x4 << ADS1118_DR_BIT) /* 128 SPS (默认) */
#define ADS1118_DR_250SPS (0x5 << ADS1118_DR_BIT) /* 250 SPS */
#define ADS1118_DR_475SPS (0x6 << ADS1118_DR_BIT) /* 475 SPS */
#define ADS1118_DR_860SPS (0x7 << ADS1118_DR_BIT) /* 860 SPS */

/* Bit 4: TS_MODE (测量内部温度还是输入信号) */
#define ADS1118_TS_MODE_BIT  4
#define ADS1118_TS_MODE_ADC  (0 << ADS1118_TS_MODE_BIT) /* ADC模式 (默认) */
#define ADS1118_TS_MODE_TEMP (1 << ADS1118_TS_MODE_BIT) /* 温度传感器模式 */

/* Bit 3: PULL_UP_EN (是否在输出端口使用上拉电阻) */
#define ADS1118_PUD_BIT 3
#define ADS1118_PUD_DISABLED                                                   \
    (0 << ADS1118_PUD_BIT) /* 禁用上拉电阻 (DOUT/DRDY) */
#define ADS1118_PUD_ENABLED                                                    \
    (1 << ADS1118_PUD_BIT) /* 启用内部上拉电阻 (默认)                \
                            */

/* Bits 2-0: NOP (指令是否写入寄存器) */
#define ADS1118_NOP_BIT     1
#define ADS1118_NOP_INVALID (0x0 << ADS1118_NOP_BIT) /* 不修改配置寄存器 */
#define ADS1118_NOP_VALID   (0x1 << ADS1118_NOP_BIT) /* 修改配置寄存器 (默认) */

/**
 * @brief ADS1118配置寄存器默认值
 * 包含所有配置位的默认设置组合
 */
#define ADS11118_CONFIG_REG_DEFAULT                                            \
    (ADS1118_SS_START_CONV | ADS1118_MUX_AIN0_AIN1 | ADS1118_PGA_2_048V |      \
     ADS1118_MODE_SINGLE | ADS1118_DR_128SPS | ADS1118_TS_MODE_ADC |           \
     ADS1118_PUD_ENABLED | ADS1118_NOP_VALID)

/**
 * @brief 向ADS1118芯片写入寄存器值
 * @param reg_value 要写入的16位寄存器值
 * @retval 无
 */
void ads1118_write(uint16_t reg_value);

/**
 * @brief 从ADS1118芯片读取ADC转换结果
 * @return 16位ADC转换数据
 */
uint16_t ads1118_read(void);

/**
 * @brief 与ADS1118芯片进行SPI通信
 * @param reg_value 要发送到芯片的寄存器值
 * @param data 从芯片接收到的数据的指针
 * @retval 无
 */
void ads1118_communicate(uint16_t reg_value, uint16_t *data);

/**
 * @brief 初始化ADS1118模块
 * 配置ADS1118的默认设置
 */
void ads1118_initialize(void);

#endif /* ADS1118_H */