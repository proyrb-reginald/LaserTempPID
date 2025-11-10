#include <ads1118.h>

typedef struct
{
    ADS1118_UINT8_TYPE SS : 1;         // 0无效 1开启单次转换(仅限省电模式)
    ADS1118_UINT8_TYPE MUX : 3;        // 配置输入引脚
    ADS1118_UINT8_TYPE PGA : 3;        // 配置量程
    ADS1118_UINT8_TYPE MODE : 1;       // 0连续转换模式 1省点模式
    ADS1118_UINT8_TYPE DR : 3;         // 配置采样率
    ADS1118_UINT8_TYPE TS_MODE : 1;    // 0采集外部引脚 1采集内部温度
    ADS1118_UINT8_TYPE PULL_UP_EN : 1; // 0不启用弱上拉 1启用弱上拉
    ADS1118_UINT8_TYPE NOP : 2;        // 1更新寄存器 x不更新寄存器
    ADS1118_UINT8_TYPE Reserved : 1;   // 无效
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

static ADS1118_UINT16_TYPE ads1118_get_config_value(void)
{
    ADS1118_UINT16_TYPE value = 0;
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
    config.MODE = 0; // 连续转换
    config.DR = 7;   // 860 SPS
    ADS1118_UINT16_TYPE value = ads1118_get_config_value();

    ADS1118_UINT8_TYPE tx_data[4] = {0};
    tx_data[0] = ((ADS1118_UINT16_TYPE)value >> 8) & 0x00FF;
    tx_data[1] = (ADS1118_UINT16_TYPE)value & 0x00FF;
    tx_data[2] = ((ADS1118_UINT16_TYPE)value >> 8) & 0x00FF;
    tx_data[3] = (ADS1118_UINT16_TYPE)value & 0x00FF;
    ADS1118_LOG_INTERFACE("tx_data:0x%X%X%X%X\n", tx_data[0], tx_data[1],
                          tx_data[2], tx_data[3]);

    ADS1118_UINT8_TYPE rx_data[4] = {0};
    ads1118_gpio_cs_reset();
    HAL_SPI_TransmitReceive(&hspi3, tx_data, rx_data, 4, 10);
    ads1118_gpio_cs_set();
    ADS1118_LOG_INTERFACE("rx_data:0x%X%X%X%X\n", rx_data[0], rx_data[1],
                          rx_data[2], rx_data[3]);
}

void ads1118_set_channel(ADS1118_UINT8_TYPE channel)
{
    config.MUX = channel;
    ADS1118_UINT16_TYPE value = ads1118_get_config_value();
    ADS1118_UINT8_TYPE tx_data[2] = {0};
    tx_data[0] = ((ADS1118_UINT16_TYPE)value >> 8) & 0x00FF;
    tx_data[1] = (ADS1118_UINT16_TYPE)value & 0x00FF;
    ads1118_gpio_cs_reset();
    HAL_SPI_Transmit(&hspi3, tx_data, 2, 10);
    ads1118_gpio_cs_set();
}

float ads1118_read_channel(void)
{
    volatile ADS1118_UINT8_TYPE data[2] = {0};
    ads1118_gpio_cs_reset();
    ads1118_spi_receive((ADS1118_UINT8_TYPE *)data, 2);
    ads1118_gpio_cs_set();
    ADS1118_LOG_INTERFACE("adc:0x%X%x\n", data[0], data[1]);
    ADS1118_UINT16_TYPE value = (((ADS1118_UINT16_TYPE)data[0] << 8) & 0xFF00) |
                                ((ADS1118_UINT16_TYPE)data[1] & 0x00FF);
    float rate = 0.0f;
    rate = ((float)value / 65535) * ADS1118_VOLTAGE_VREF;
    return rate;
}