#ifndef BOARD_H
#define BOARD_H

#include <stm32h7xx_hal.h>
#include <main.h>
#include <gpio.h>
#include <spi.h>
#include <usart.h>
#include <adc.h>
#include <dma.h>

extern void rt_os_tick_callback(void);
extern int rt_lprintf(const char *fmt, ...);

#endif