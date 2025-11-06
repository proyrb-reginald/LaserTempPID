#include <dacx0502.h>

#define DAC80502 16
#define DAC70502 14
#define DAC60502 12

#if DACX0502_DEVICE == DAC80502
#define DACX0502_DAC_MAX 0xFFFF
#elif DACX0502_DEVICE == DAC70502
#define DACX0502_DAC_MAX 0x3FFF
#elif DACX0502_DEVICE == DAC60502
#define DACX0502_DAC_MAX 0x0FFF
#endif

/* DACX0502寄存器地址 */
#define DACX0502_CMD_DEVID 0x1
#define DACX0502_CMD_SYNC 0x2
#define DACX0502_CMD_CONFIG 0x3
#define DACX0502_CMD_GAIN 0x4
#define DACX0502_CMD_TRIGGER 0x5
#define DACX0502_CMD_BRDCAST 0x6
#define DACX0502_CMD_STATUS 0x7
#define DACX0502_CMD_DAC_A 0x8
#define DACX0502_CMD_DAC_B 0x9

typedef struct dacx0502_devid_reg
{
    DACX0502_UINT16_TYPE reserved_15 : 1;
    DACX0502_UINT16_TYPE resolution : 3;
    DACX0502_UINT16_TYPE reserved_11_0 : 12;
} dacx0502_devid_reg;

typedef struct dacx0502_sync_reg
{
    DACX0502_UINT16_TYPE reserved_15_10 : 6;
    DACX0502_UINT16_TYPE dac_b_brdcast_en : 1;
    DACX0502_UINT16_TYPE dac_a_brdcast_en : 1;
    DACX0502_UINT16_TYPE reserved_7_2 : 6;
    DACX0502_UINT16_TYPE dac_b_sync_en : 1;
    DACX0502_UINT16_TYPE dac_a_sync_en : 1;
} dacx0502_sync_reg;

typedef struct dacx0502_config_reg
{
    DACX0502_UINT16_TYPE reserved_15_9 : 7;
    DACX0502_UINT16_TYPE ref_pwdwn : 1;
    DACX0502_UINT16_TYPE reserved_7_2 : 6;
    DACX0502_UINT16_TYPE dac_b_pwdwn : 1;
    DACX0502_UINT16_TYPE dac_a_pwdwn : 1;
} dacx0502_config_reg;

typedef struct dacx0502_gain_reg
{
    DACX0502_UINT16_TYPE reserved_15_9 : 7;
    DACX0502_UINT16_TYPE ref_div : 1;
    DACX0502_UINT16_TYPE reserved_7_2 : 6;
    DACX0502_UINT16_TYPE buff_b_gain : 1;
    DACX0502_UINT16_TYPE buff_a_gain : 1;
} dacx0502_gain_reg;

typedef struct dacx0502_trigger_reg
{
    DACX0502_UINT16_TYPE reserved_15_5 : 11;
    DACX0502_UINT16_TYPE ldac : 1;
    DACX0502_UINT16_TYPE soft_reset : 4;
} dacx0502_trigger_reg;

typedef struct dacx0502_brdcast_reg
{
    DACX0502_UINT16_TYPE brdcast_data;
} dacx0502_brdcast_reg;

typedef struct dacx0502_status_reg
{
    DACX0502_UINT16_TYPE ref_alarm : 15;
    DACX0502_UINT16_TYPE reserved_0 : 1;
} dacx0502_status_reg;

typedef struct dacx0502_daca_reg
{
    DACX0502_UINT16_TYPE dac_a_data;
} dacx0502_daca_reg;

typedef struct dacx0502_dacb_reg
{
    DACX0502_UINT16_TYPE dac_b_data;
} dacx0502_dacb_reg;

typedef struct dacx0502_reg_map
{
    dacx0502_devid_reg devid_reg;
    dacx0502_sync_reg sync_reg;
    dacx0502_config_reg config_reg;
    dacx0502_gain_reg gain_reg;
    dacx0502_trigger_reg trigger_reg;
    dacx0502_brdcast_reg brdcast_reg;
    dacx0502_status_reg status_reg;
    dacx0502_daca_reg daca_reg;
    dacx0502_dacb_reg dacb_reg;
} dacx0502_reg_map;

static dacx0502_reg_map reg_map = {0};

void dacx0502_init(void)
{
    uint8_t tx_data[3] = {0};

    /* 软件复位 */
    tx_data[0] = DACX0502_CMD_TRIGGER;
    tx_data[1] = 0x00;
    tx_data[2] = 0x0A;
    dacx0502_spi_transmit(tx_data, 3);
    rt_thread_delay(1);

    /* 设置参考电压源 */
    tx_data[0] = DACX0502_CMD_CONFIG;
    tx_data[1] = 0x00 | DACX0502_USE_EXTERNAL_REF_VOLTAGE;
    tx_data[2] = 0x00;
    dacx0502_spi_transmit(tx_data, 3);
    rt_thread_delay(1);

    /* 设置电压衰减和增益 */
    tx_data[0] = DACX0502_CMD_GAIN;
    tx_data[1] = 0x00;
    tx_data[2] = 0x00;
    dacx0502_spi_transmit(tx_data, 3);
    rt_thread_delay(1);
}

void dacx0502_set_dac_a(float rate)
{
    DACX0502_UINT16_TYPE data = 0;
    {
        int32_t tmp = rate * DACX0502_DAC_MAX;
        if (tmp > (int32_t)DACX0502_DAC_MAX)
        {
            data = DACX0502_DAC_MAX;
        }
        else if (tmp < 0)
        {
            data = 0;
        }
        else
        {
            data = tmp;
        }
        data = data << (16 - DACX0502_DEVICE);
    }

    uint8_t tx_data[3] = {0};
    tx_data[0] = 0x00 | DACX0502_CMD_DAC_A;
    tx_data[1] = (data >> 8) & 0x00FF;
    tx_data[2] = data & 0x00FF;
    dacx0502_spi_transmit(tx_data, 3);
}

void dacx0502_set_dac_b(float rate)
{
    DACX0502_UINT16_TYPE data = 0;
    {
        int32_t tmp = rate * DACX0502_DAC_MAX;
        if (tmp > (int32_t)DACX0502_DAC_MAX)
        {
            data = DACX0502_DAC_MAX;
        }
        else if (tmp < 0)
        {
            data = 0;
        }
        else
        {
            data = tmp;
        }
        data = data << (16 - DACX0502_DEVICE);
    }

    uint8_t tx_data[3] = {0};
    tx_data[0] = 0x00 | DACX0502_CMD_DAC_B;
    tx_data[1] = (data >> 8) & 0x00FF;
    tx_data[2] = data & 0x00FF;
    dacx0502_spi_transmit(tx_data, 3);
}