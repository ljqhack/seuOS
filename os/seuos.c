#include "seuos.h"

static INT32U OSRdyTbl;
static INT8U OSTaskRunningPrio;

TCB_t OSTCB[32];

void OSTaskCreate(void(*task)(void), OS_STK_t *top, INT8U prio)
{
    OS_STK *stk;
    stk = top;

    *(stk) = (INT32U)0x01000000L;
    *(--stk) = (INT32U)task;
    *(--stk) = (INT32U)0xFFFFFFFFL;
    *(--stk) = (INT32U)0x12121212L;
    *(--stk) = (INT32U)0x03030303L;
    *(--stk) = (INT32U)0x01010101L;
 
    *(--stk) = (INT32U)0x11111111L;
    *(--stk) = (INT32U)0x10101010L;
    *(--stk) = (INT32U)0x09090909L;
    *(--stk) = (INT32U)0x08080808L;
    *(--stk) = (INT32U)0x07070707L;
    *(--stk) = (INT32U)0x06060606L;
    *(--stk) = (INT32U)0x05050505L;
    *(--stk) = (INT32U)0x04040404L;
    
    OSTCB[prio].OSTaskStackTop = stk;
    OSRdyTbl |= (0x01)<<prio;
}

void OSStartTask()
{
    OSTaskRunningPrio = 32;
    
}

