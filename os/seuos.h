#ifndef __SEUOS_H__
#define __SEUOS_H__
#include "os_def.h"
#include <stdbool.h>

extern INT32U SystemCoreClock;

#define SYSTICK_CTRL		( ( volatile INT32U *) 0xe000e010 )
#define SYSTICK_LOAD		( ( volatile INT32U *) 0xe000e014 )
#define configCPU_CLOCK_HZ				( SystemCoreClock )
#define configTICK_RATE_HZ				( ( INT32U ) 1000 )
#define portNVIC_SYSTICK_CLK		0x00000004
#define portNVIC_SYSTICK_INT		0x00000002
#define portNVIC_SYSTICK_ENABLE		0x00000001

#define DisInt()				__disable_irq()
#define EnInt()					__enable_irq()

extern void OSInit(void);
extern void OSTimeDly(INT32U i);
extern void OSTaskCreate(void(*task)(void), OS_STK_t *top, INT8U prio);
extern void OSStartTask(void);

/****declare assembler function prototype****/
//extern void OSCtxSw(void);
extern void OSStartHighRdy(void);
extern void OSCtxSw(void);
#endif

