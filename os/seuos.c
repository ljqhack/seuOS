#include "seuos.h"

INT32U OSTaskTbl;			//if task is created, set relevant bit
INT32U OSRdyTbl;				//task ready table
INT8U OSTaskRunningPrio;		//runing priority
INT8U OSPrioHighRdy;
TCB_t *OSCurrentTCB;
TCB_t *OSPrioHighTCB;
BOOLEAN OSRunning;			//OS runing flag

TCB_t OSTCB[32];

OS_STK_t StackIDLE[50];

static void TaskIdle()
{
	INT32U i;
	while(1)
	{
		i++;
	}
}

void OSCPUSystickInit( void )
{
	*(SYSTICK_LOAD) = ( configCPU_CLOCK_HZ / configTICK_RATE_HZ ) - 1UL;
	*(SYSTICK_CTRL) = portNVIC_SYSTICK_CLK | portNVIC_SYSTICK_INT | portNVIC_SYSTICK_ENABLE;
}

void OSInit(void)
{
    OSRunning = false;
		OSTaskCreate(TaskIdle, &StackIDLE[49], 31);					//Init Idle Task
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
		OSCPUSystickInit();
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

void OSTimeTick()
{
	
}

void SysTick_Handler(void)
{
	OSTimeTick();
}