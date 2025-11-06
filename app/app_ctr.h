#ifndef APP_CTR_H
#define APP_CTR_H

#include <rtthread.h>

#define CTR_TASK_NAME   "ctr"
#define CTR_CURRENT     40.0f // 单位mA
#define CTR_TEMPERATURE 28.0f // 单位℃

typedef struct app_ctr_args {
    rt_uint8_t current_mode : 1;
    rt_uint8_t temperature_mode : 1;
    float current;
    float temperature;
} app_ctr_args;

void thread_ctr(void *parameter);

#endif /* APP_CTR_H */