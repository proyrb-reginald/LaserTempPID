#include <app_cmd.h>
#include <app_ctr.h>
#include <errno.h>
#include <rtthread.h>
#include <stdlib.h>
#include <string.h>

static app_ctr_args ctr_args = {.current_mode = 1,
                                .temperature_mode = 0,
                                .current = CTR_CURRENT,
                                .temperature = CTR_TEMPERATURE};

static rt_thread_t ctr_tid = RT_NULL;
static rt_thread_t mon_tid = RT_NULL;

int msh_start_app(int argc, char **argv)
{
    if (strcmp(CTR_TASK_NAME, argv[1]) == 0)
    {
        ctr_tid =
            rt_thread_create(CTR_TASK_NAME, thread_ctr, &ctr_args, 512, 4, 0);
        if (ctr_tid != RT_NULL)
        {
            if (rt_thread_startup(ctr_tid) == RT_EOK)
            {
                rt_kprintf("Thread[%s] started.\n", ctr_tid->name);
            }
            else
            {
                rt_kprintf("Failed to start thread[%s].\n", ctr_tid->name);
                rt_thread_delete(ctr_tid);
            }
        }
        else
        {
            rt_kprintf("Failed to create thread[%s].\n", ctr_tid->name);
        }
        return 0;
    }
    else if (strcmp(MON_TASK_NAME, argv[1]) == 0)
    {
        mon_tid =
            rt_thread_create(MON_TASK_NAME, thread_mon, RT_NULL, 512, 5, 0);
        if (mon_tid != RT_NULL)
        {
            if (rt_thread_startup(mon_tid) == RT_EOK)
            {
                rt_kprintf("Thread[%s] started.\n", mon_tid->name);
            }
            else
            {
                rt_kprintf("Failed to start thread[%s].\n", mon_tid->name);
                rt_thread_delete(mon_tid);
            }
        }
        else
        {
            rt_kprintf("Failed to create thread[%s].\n", mon_tid->name);
        }
        return 0;
    }

    rt_kprintf("Usage: start_app [%s|%s]\n", CTR_TASK_NAME, MON_TASK_NAME);
    return 0;
}
MSH_CMD_EXPORT_ALIAS(msh_start_app, start_app, Usage : start_app[task_name]);

int msh_exit_app(int argc, char **argv)
{
    if (strcmp(CTR_TASK_NAME, argv[1]) == 0)
    {
        if (ctr_tid != RT_NULL)
        {
            if (rt_thread_delete(ctr_tid) == RT_EOK)
            {
                rt_kprintf("Thread[%s] deleted.\n", ctr_tid->name);
                return 0;
            }
        }
        rt_kprintf("Failed to delete thread[%s].\n", ctr_tid->name);
        return 0;
    }
    else if (strcmp(MON_TASK_NAME, argv[1]) == 0)
    {
        if (mon_tid != RT_NULL)
        {
            if (rt_thread_delete(mon_tid) == RT_EOK)
            {
                rt_kprintf("Thread[%s] deleted.\n", mon_tid->name);
                return 0;
            }
        }
        rt_kprintf("Failed to delete thread[%s].\n", mon_tid->name);
        return 0;
    }

    rt_kprintf("Usage: exit_app [%s|%s]\n", CTR_TASK_NAME, MON_TASK_NAME);
    return 0;
}
MSH_CMD_EXPORT_ALIAS(msh_exit_app, exit_app, Usage : exit_app[task_name]);
