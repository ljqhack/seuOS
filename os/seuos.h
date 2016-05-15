#ifndef __SEUOS_H__
#define __SEUOS_H__
#include "os_def.h"

typedef INT32U OS_STK_t;
typedef struct TCB{
    OS_STK_t OSTaskStackTop;
    INT32U OSWaitTick;
}TCB_t;

extern void OSInit(void);
extern void OSTimeDly(INT32U i);
extern void OSTaskCreate(void(*task)(void), OS_STK_t *top, INT8U prio);
extern void OSStartTask(void);

extern void OS_Sched(void);

/****declare assembler function prototype****/
extern void OSCtxSw(void);

#endif

