#define ADS1118_C

#include <ads1118.h>

typedef struct
{
    uint8_t SS : 1;         // 0不起作用 1开启单次转换(仅当省电模式有效)
    uint8_t MUX : 3;        // 配置输入引脚
    uint8_t PGA : 3;        // 配置量程
    uint8_t MODE : 1;       // 0连续转换模式 1省点模式
    uint8_t DR : 3;         // 配置采样率
    uint8_t TS_MODE : 1;    // 0采集外部引脚 1采集内部温度
    uint8_t PULL_UP_EN : 1; // 0不启用弱上拉 1启用弱上拉
    uint8_t NOP : 2;        // 1更新寄存器 x不更新寄存器
    uint8_t Reserved : 1;   // 不起作用
} ads1118_config;

static ads1118_config config = {
    .SS = 0,
    .MUX = 0,
    .PGA = 2,
    .MODE = 1,
    .DR = 4,
    .TS_MODE = 0,
    .PULL_UP_EN = 1,
    .NOP = 1,
    .Reserved = 1,
};

static uint16_t ads1118_get_config_value(void)
{
    uint16_t value = 0;
    value |= config.SS << 15;
    value |= config.MUX << 12;
    value |= config.PGA << 9;
    value |= config.MODE << 8;
    value |= config.DR << 5;
    value |= config.TS_MODE << 4;
    value |= config.PULL_UP_EN << 3;
    value |= config.NOP << 1;
    return value;
}

void ads1118_init(void)
{
    // 0110 0010 1110 1010
    // config.MUX = 6;  // AIN2-GND
    config.MUX = 7;  // AIN3-GND
    config.PGA = 1;  // 4.096V
    config.MODE = 0; // 连续转换
    config.DR = 7;   // 860 SPS
    ADS1118_UINT16_TYPE value = ads1118_get_config_value();

    ADS1118_UINT8_TYPE tx_data[4] = {0};
    tx_data[0] = ((ADS1118_UINT16_TYPE)value >> 8) & 0x00FF;
    tx_data[1] = (ADS1118_UINT16_TYPE)value & 0x00FF;
    tx_data[2] = ((ADS1118_UINT16_TYPE)value >> 8) & 0x00FF;
    tx_data[3] = (ADS1118_UINT16_TYPE)value & 0x00FF;
    rt_lprintf("config:0x%X%X\n", tx_data[0], tx_data[1]);

    ADS1118_UINT8_TYPE rx_data[4] = {0};
    ads1118_gpio_cs_reset();
    HAL_SPI_TransmitReceive(&hspi3, tx_data, rx_data, 4, 20);
    ads1118_gpio_cs_set();
    rt_lprintf("re:0x%X%X 0x%X%X\n", rx_data[0], rx_data[1], rx_data[2],
               rx_data[3]);
}

void ads1118_read_ain(void)
{
    volatile uint8_t data[2] = {0};
    ads1118_gpio_cs_reset();
    ads1118_spi_receive((uint8_t *)data, 2);
    ads1118_gpio_cs_set();
    rt_lprintf("ain2:0x%X%x\n", data[0], data[1]);
}