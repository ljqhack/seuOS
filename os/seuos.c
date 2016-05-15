#include "seuos.h"

static INT32U OSTaskTbl;			//if task is created, set relevant bit
static INT32U OSRdyTbl;				//task ready table
static INT8U OSTaskRunningPrio;		//runing priority
static INT8U OSPrioHighRdy;
static BOOLEAN OSRunning;			//OS runing flag

TCB_t OSTCB[32];

void OSInit(void)
{
    OSRunning = FALSE;
}

void OSTimeDly(INT32U i)
{
	
}

void OSTaskCreate(void(*task)(void), OS_STK_t *top, INT8U prio)
{
    OS_STK *stk;
	if( ( OSTaskTbl & (0x00000001L<<prio)) ) 
	{
		return;						//Task priority has been created, fail
	}
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
	OSTaskTbl |= (0x00000001L)<<prio;
    OSRdyTbl |= (0x00000001)<<prio;
}

void OSStartTask(void)
{
	if(FALSE == OSRunning)
	{
		OSPrioHighRdy = 0;
		while( !(OSRdyTbl & ((0x00000001L)<<OSPrioHighRdy)) )
		{
			OSPrioHighRdy++;
		}
		OSTaskRunningPrio = OSPrioHighRdy;
		OSStartHighRdy();
	}
}

void OS_Sched(void)
{
	OSPrioHighRdy = 0;
	while( !(OSRdyTbl & ((0x00000001L)<<OSPrioHighRdy)) )
	{
		OSPrioHighRdy++;
	}
	if(OSPrioHighRdy != OSTaskRunningPrio)
	{
		OSTaskRunningPrio = OSPrioHighRdy;
		OSCtxSw();
	}
}

