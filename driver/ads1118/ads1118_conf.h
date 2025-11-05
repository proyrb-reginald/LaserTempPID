#ifndef ADS1118_CONF_H
#define ADS1118_CONF_H

#include "main.h"

/* --- 用户配置区 --- */

/* SPI Handle: 指定用于与ADS1118通信的SPI实例 */
extern SPI_HandleTypeDef hspi3; /* 请根据实际情况修改 */
#define ADS1118_SPI_HANDLE hspi3

/* GPIO & CS Pin: 定义ADWS1118的CS引脚 */
#define ADS1118_CS_PORT GPIOA
#define ADS1118_CS_PIN  GPIO_PIN_15

/* 读取超时: 在等待DRDY信号或SPI传输时的超时时间 (单位: ms) */
#define ADS1118_TIMEOUT_MS 20

/* 温度传感器模式: 如果您的应用需要使用内部温度传感器，取消下面的注释 */
/* #define ADS1118_USE_INTERNAL_TEMP_SENSOR */

/* --- 配置校验 --- */

/* 检查是否定义了SPI句柄 */
#ifndef ADS1118_SPI_HANDLE
#error "ADS1118: Please define ADS1118_SPI_HANDLE in ads1118_conf.h"
#endif

/* 检查是否定义了CS引脚 */
#ifndef ADS1118_CS_PORT
#ifndef ADS1118_CS_PIN
#error                                                                         \
    "ADS1118: Please define ADS1118_CS_PORT and ADS1118_CS_PIN in ads1118_conf.h"
#endif
#endif

#endif /* ADS1118_CONF_H */