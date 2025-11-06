#include <mcp4728.h>

#define MCP4728_DAC_MAX (4096 - 1)
#define MCP4728_CX(x) (MCP4728_UINT8_TYPE)(x)
#define MCP4728_WX(x) (MCP4728_UINT8_TYPE)(x)
#define MCP4728_DACX(x) (MCP4728_UINT8_TYPE)(x)
#define MCP4728_VREFX(x) (MCP4728_UINT8_TYPE)(x)
#define MCP4728_PDX(x) (MCP4728_UINT8_TYPE)(x)
#define MCP4728_GX(x) (MCP4728_UINT8_TYPE)(x)

static MCP4728_UINT8_TYPE mcp4728_addr_eeprom = 0b01100000;
static MCP4728_UINT8_TYPE mcp4728_addr_register = 0b01100000;

void mcp4728_init(void)
{
    /* 设备复位 */
    mcp4728_i2c_transmit(0x0, (MCP4728_UINT8_TYPE[]){0x06}, 1);
    MCP4728_DELAY_INTERFACE(1);

    /* 设备唤醒 */
    mcp4728_i2c_transmit(0x0, (MCP4728_UINT8_TYPE[]){0x09}, 1);
    MCP4728_DELAY_INTERFACE(1);

    /* 读取地址位 */
    MCP4728_UINT8_TYPE data[1] = {0};
    mcp4728_gpio_ldac_set();
    mcp4728_i2c_transmit(0x0, (MCP4728_UINT8_TYPE[]){0x0C}, 1);
    mcp4728_gpio_ldac_reset();
    mcp4728_i2c_transmit((0b1100000 << 1) | 0x1, data, 1);
    mcp4728_gpio_ldac_set();
    mcp4728_addr_eeprom = 0b01100000 | ((data[0] >> 5) & 0x07);
    mcp4728_addr_register = 0b01100000 | ((data[0] >> 1) & 0x07);
    MCP4728_LOG_INTERFACE("0x%X(RAW) 0x%X(EEPROM) 0x%X(Register)\n", data[0],
                          mcp4728_addr_eeprom, mcp4728_addr_register);

    /* 设置通道增益 */
    data[0] = 0;
    data[0] |= (MCP4728_CX(6) << 5) & 0xE0;
    data[0] |= (0b0 << 3) & 0x08; // Gain 1 for A
    data[0] |= (0b0 << 2) & 0x04; // Gain 1 for B
    data[0] |= (0b0 << 1) & 0x02; // Gain 1 for C
    data[0] |= (0b0 << 0) & 0x01; // Gain 1 for D
    mcp4728_i2c_transmit((mcp4728_addr_register << 1 | 0x0), data, 1);
}

void mcp4728_set_dac_a(float rate)
{
    MCP4728_INT16_TYPE tmp = rate * MCP4728_DAC_MAX;
    MCP4728_UINT16_TYPE value;
    if (tmp > (MCP4728_INT16_TYPE)MCP4728_DAC_MAX)
    {
        value = MCP4728_DAC_MAX;
    }
    else if (tmp < 0)
    {
        value = 0;
    }
    else
    {
        value = tmp;
    }
    MCP4728_UINT8_TYPE data[3] = {0};
    data[0] |= (MCP4728_CX(2) << 5) & 0xE0;
    data[0] |= (MCP4728_WX(0) << 3) & 0x18;
    data[0] |= (MCP4728_DACX(0) << 1) & 0x06;
    data[0] |= (0 << 0) & 0x01;
    data[1] |= (MCP4728_VREFX(1) << 7) & 0x80;
    data[1] |= (MCP4728_PDX(0) << 5) & 0x60;
    data[1] |= (MCP4728_GX(0) << 4) & 0x10;
    data[1] |= (value >> 8) & 0x0F;
    data[2] = value & 0x00FF;
    mcp4728_i2c_transmit((mcp4728_addr_register << 1 | 0x0), data, 3);
}

void mcp4728_set_dac_b(float rate)
{
    MCP4728_INT16_TYPE tmp = rate * MCP4728_DAC_MAX;
    MCP4728_UINT16_TYPE value;
    if (tmp > (MCP4728_INT16_TYPE)MCP4728_DAC_MAX)
    {
        value = MCP4728_DAC_MAX;
    }
    else if (tmp < 0)
    {
        value = 0;
    }
    else
    {
        value = tmp;
    }
    MCP4728_UINT8_TYPE data[3] = {0};
    data[0] |= (MCP4728_CX(2) << 5) & 0xE0;
    data[0] |= (MCP4728_WX(0) << 3) & 0x18;
    data[0] |= (MCP4728_DACX(1) << 1) & 0x06;
    data[0] |= (0 << 0) & 0x01;
    data[1] |= (MCP4728_VREFX(1) << 7) & 0x80;
    data[1] |= (MCP4728_PDX(0) << 5) & 0x60;
    data[1] |= (MCP4728_GX(0) << 4) & 0x10;
    data[1] |= (value >> 8) & 0x0F;
    data[2] = value & 0x00FF;
    mcp4728_i2c_transmit((mcp4728_addr_register << 1 | 0x0), data, 3);
}