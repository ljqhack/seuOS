#include "seuos.h"

INT32U OSTaskTbl;			//if task is created, set relevant bit
INT32U OSRdyTbl;				//task ready table
INT8U OSTaskRunningPrio;		//runing priority
INT8U OSPrioHighRdy;
TCB_t *OSCurrentTCB;
TCB_t *OSPrioHighTCB;
BOOLEAN OSRunning;			//OS runing flag

TCB_t OSTCB[32];

void OSInit(void)
{
    OSRunning = false;
}

void OSTimeDly(INT32U ticks)
{
	if(ticks > 0)
	{
		OSRdyTbl &= ~( (0x00000001)<<OSTaskRunningPrio );
		OSTCB[OSTaskRunningPrio].OSWaitTick = ticks;
		OS_Sched();
	}
}

void OSTaskCreate(void(*task)(void), OS_STK_t *top, INT8U prio)
{
    OS_STK_t *stk;
		if( ( OSTaskTbl & (0x00000001L<<prio)) ) 
		{
			return;						//Task priority has been created, fail
		}
    stk = top;
		--stk;
		*stk = 0x01000000;
		--stk;
		*stk = (INT32U) task;
		--stk;
		*stk = 0x00000000;
		stk -= 5;
		*stk = 0x00000000;
		stk -= 8;
    
    OSTCB[prio].OSTaskStackTop = stk;
		OSTaskTbl |= (0x00000001L)<<prio;
    OSRdyTbl |= (0x00000001L)<<prio;
}

void OSStartTask(void)
{
	if(false == OSRunning)
	{
		OSPrioHighRdy = 0;
		while( !(OSRdyTbl & ((0x00000001L)<<OSPrioHighRdy)) )
		{
			OSPrioHighRdy++;
		}
		OSTaskRunningPrio = OSPrioHighRdy;
		OSPrioHighTCB = OSTCB + OSPrioHighRdy;
		OSCurrentTCB = OSPrioHighTCB;
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
		OSPrioHighTCB = OSTCB + OSPrioHighRdy;
		OSCtxSw();
	}
}

