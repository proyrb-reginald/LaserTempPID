#include "mcp4728.h"

uint8_t mcp4728_init(void)
{
    /* 检查设备是否响应 */
    HAL_StatusTypeDef status = HAL_I2C_IsDeviceReady(
        &MCP4728_I2C_HANDLE, MCP4728_I2C_ADDR << 1, 3, MCP4728_TIMEOUT_MS);

    return (status == HAL_OK) ? 0 : 1;
}

uint8_t mcp4728_fast_write(uint8_t channel, uint16_t data)
{
    uint8_t buffer[3];

    /* 确保数据不超过12位 */
    data &= 0x0FFF;

    /* 构造快速写命令 */
    buffer[0] = MCP4728_CMD_FAST_WRITE | (channel << 1) | ((data >> 8) & 0x0F);
    buffer[1] = data & 0xFF;

    /* 发送数据 */
    HAL_StatusTypeDef status =
        HAL_I2C_Master_Transmit(&MCP4728_I2C_HANDLE, MCP4728_I2C_ADDR << 1,
                                buffer, 2, MCP4728_TIMEOUT_MS);

    return (status == HAL_OK) ? 0 : 1;
}

uint8_t mcp4728_write_channel_reg(uint8_t channel,
                                  const mcp4728_channel_config_t *config)
{
    uint8_t buffer[3];

    /* 确保数据不超过12位 */
    uint16_t data = config->data & 0x0FFF;

    /* 构造写寄存器命令 */
    buffer[0] = MCP4728_CMD_WRITE_REG | (channel << 1);
    buffer[1] = ((config->ref_mode & 0x01) << 7) |
                ((config->power_mode & 0x03) << 5) |
                ((config->gain & 0x01) << 4) | ((data >> 8) & 0x0F);
    buffer[2] = data & 0xFF;

    /* 发送数据 */
    HAL_StatusTypeDef status =
        HAL_I2C_Master_Transmit(&MCP4728_I2C_HANDLE, MCP4728_I2C_ADDR << 1,
                                buffer, 3, MCP4728_TIMEOUT_MS);

    return (status == HAL_OK) ? 0 : 1;
}

uint8_t mcp4728_write_channel_eeprom(uint8_t channel,
                                     const mcp4728_channel_config_t *config)
{
    uint8_t buffer[3];

    /* 确保数据不超过12位 */
    uint16_t data = config->data & 0x0FFF;

    /* 构造写EEPROM命令 */
    buffer[0] = MCP4728_CMD_WRITE_EEPROM | (channel << 1);
    buffer[1] = ((config->ref_mode & 0x01) << 7) |
                ((config->power_mode & 0x03) << 5) |
                ((config->gain & 0x01) << 4) | ((data >> 8) & 0x0F);
    buffer[2] = data & 0xFF;

    /* 发送数据 */
    HAL_StatusTypeDef status =
        HAL_I2C_Master_Transmit(&MCP4728_I2C_HANDLE, MCP4728_I2C_ADDR << 1,
                                buffer, 3, MCP4728_TIMEOUT_MS);

    return (status == HAL_OK) ? 0 : 1;
}

uint8_t mcp4728_write_all_channels_reg(const mcp4728_channel_config_t *configs)
{
    uint8_t buffer[9]; /* 1个命令字节 + 4个通道 * 2字节 */

    /* 构造连续写命令 */
    buffer[0] = MCP4728_CMD_WRITE_REG | 0x08; /* 多通道写入模式 */

    for (int i = 0; i < 4; i++) {
        uint16_t data     = configs[i].data & 0x0FFF;
        buffer[1 + i * 2] = ((configs[i].ref_mode & 0x01) << 7) |
                            ((configs[i].power_mode & 0x03) << 5) |
                            ((configs[i].gain & 0x01) << 4) |
                            ((data >> 8) & 0x0F);
        buffer[2 + i * 2] = data & 0xFF;
    }

    /* 发送数据 */
    HAL_StatusTypeDef status =
        HAL_I2C_Master_Transmit(&MCP4728_I2C_HANDLE, MCP4728_I2C_ADDR << 1,
                                buffer, 9, MCP4728_TIMEOUT_MS);

    return (status == HAL_OK) ? 0 : 1;
}

uint8_t mcp4728_write_all_channels_eeprom(
    const mcp4728_channel_config_t *configs)
{
    uint8_t buffer[9]; /* 1个命令字节 + 4个通道 * 2字节 */

    /* 构造连续写EEPROM命令 */
    buffer[0] = MCP4728_CMD_WRITE_EEPROM | 0x08; /* 多通道写入EEPROM模式 */

    for (int i = 0; i < 4; i++) {
        uint16_t data     = configs[i].data & 0x0FFF;
        buffer[1 + i * 2] = ((configs[i].ref_mode & 0x01) << 7) |
                            ((configs[i].power_mode & 0x03) << 5) |
                            ((configs[i].gain & 0x01) << 4) |
                            ((data >> 8) & 0x0F);
        buffer[2 + i * 2] = data & 0xFF;
    }

    /* 发送数据 */
    HAL_StatusTypeDef status =
        HAL_I2C_Master_Transmit(&MCP4728_I2C_HANDLE, MCP4728_I2C_ADDR << 1,
                                buffer, 9, MCP4728_TIMEOUT_MS);

    return (status == HAL_OK) ? 0 : 1;
}

uint8_t mcp4728_read_all_channels(mcp4728_channel_config_t *configs)
{
    uint8_t buffer[24]; /* 6字节/通道 * 4通道 */

    /* 发送读命令 */
    uint8_t cmd = MCP4728_CMD_READ_REG;
    HAL_StatusTypeDef status =
        HAL_I2C_Master_Transmit(&MCP4728_I2C_HANDLE, MCP4728_I2C_ADDR << 1,
                                &cmd, 1, MCP4728_TIMEOUT_MS);

    if (status != HAL_OK) { return 1; }

    /* 读取所有通道数据 */
    status = HAL_I2C_Master_Receive(&MCP4728_I2C_HANDLE, MCP4728_I2C_ADDR << 1,
                                    buffer, 24, MCP4728_TIMEOUT_MS);

    if (status != HAL_OK) { return 1; }

    /* 解析数据 */
    for (int i = 0; i < 4; i++) {
        configs[i].ref_mode   = (buffer[1 + i * 6] >> 7) & 0x01;
        configs[i].power_mode = (buffer[1 + i * 6] >> 5) & 0x03;
        configs[i].gain       = (buffer[1 + i * 6] >> 4) & 0x01;
        configs[i].data = ((buffer[1 + i * 6] & 0x0F) << 8) | buffer[2 + i * 6];
    }

    return 0;
}