#ifndef __SEUOS_H__
#define __SEUOS_H__
#include "os_def.h"
#include <stdbool.h>

extern void OSInit(void);
extern void OSTimeDly(INT32U i);
extern void OSTaskCreate(void(*task)(void), OS_STK_t *top, INT8U prio);
extern void OSStartTask(void);

extern void OS_Sched(void);

/****declare assembler function prototype****/
//extern void OSCtxSw(void);
extern void OSStartHighRdy(void);
extern void OSCtxSw(void);
#endif

