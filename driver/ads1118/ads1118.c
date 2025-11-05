#include "ads1118.h"

void ads1118_write(uint16_t reg_value)
{
    /* 拉低CS引脚，开始SPI通信 */
    HAL_GPIO_WritePin(ADS1118_CS_PORT, ADS1118_CS_PIN, GPIO_PIN_RESET);

    /* 通过SPI接口发送寄存器值 */
    HAL_SPI_Transmit(&ADS1118_SPI_HANDLE, (uint8_t *)&reg_value, 2,
                     ADS1118_TIMEOUT_MS);

    /* 拉高CS引脚，结束SPI通信 */
    HAL_GPIO_WritePin(ADS1118_CS_PORT, ADS1118_CS_PIN, GPIO_PIN_SET);
}

uint16_t ads1118_read(void)
{
    uint16_t data;

    /* 拉低CS引脚，开始SPI通信 */
    HAL_GPIO_WritePin(ADS1118_CS_PORT, ADS1118_CS_PIN, GPIO_PIN_RESET);

    /* 通过SPI接口接收ADC转换结果 */
    HAL_SPI_Receive(&ADS1118_SPI_HANDLE, (uint8_t *)&data, 2,
                    ADS1118_TIMEOUT_MS);

    /* 拉高CS引脚，结束SPI通信 */
    HAL_GPIO_WritePin(ADS1118_CS_PORT, ADS1118_CS_PIN, GPIO_PIN_SET);

    return data;
}

void ads1118_communicate(uint16_t reg_value, uint16_t *data)
{
    /* 拉低CS引脚，开始SPI通信 */
    HAL_GPIO_WritePin(ADS1118_CS_PORT, ADS1118_CS_PIN, GPIO_PIN_RESET);

    /* 通过SPI发送并接收数据 */
    HAL_SPI_TransmitReceive(&ADS1118_SPI_HANDLE, (uint8_t *)&reg_value,
                            (uint8_t *)data, 2, ADS1118_TIMEOUT_MS);

    /* 拉高CS引脚，结束SPI通信 */
    HAL_GPIO_WritePin(ADS1118_CS_PORT, ADS1118_CS_PIN, GPIO_PIN_SET);
}

void ads1118_initialize(void)
{
    /* 拉高CS引脚，确保芯片处于未选中状态 */
    HAL_GPIO_WritePin(ADS1118_CS_PORT, ADS1118_CS_PIN, GPIO_PIN_SET);
}