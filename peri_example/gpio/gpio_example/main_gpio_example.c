/*
 * @brief General Purpose Input/Output example
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2013
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "board.h"
#include "seuos.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/* Number of tickrate per second */
#define TICKRATE_HZ (10)

#if defined(BOARD_NXP_LPCXPRESSO_824)
#define PORT_MASK       0x3
#else
/* Port mask value to select pin 0-3 */
#define PORT_MASK       0x0F
#endif

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/
void (*myfun)(void);
OS_STK_t TaskStack[100];
OS_STK_t TaskStack1[100];
OS_STK_t TaskStack2[100];
/*****************************************************************************
 * Private functions
 ****************************************************************************/

void delay(uint32_t i)
{
	uint32_t j;
	uint32_t k;
	for(j = i; j > 0; j--)
	{
		for(k = 10000; k > 0; k--);
	}
}

void task0()
{
	uint8_t i = 0;
	while(1)
	{
		i++;
		Board_UARTPutSTR("task0\r\n");
		OSTimeDly(5000);
	}
}

void task1()
{
	uint8_t i = 0;
	while(1)
	{
		i++;
		Board_UARTPutSTR("task1\r\n");
		OSTimeDly(800);
	}
}

void task2()
{
	uint8_t i = 0;
	while(1)
	{
		i++;
		Board_UARTPutSTR("task2\r\n");
		OSTimeDly(2000);
	}
}
/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief	Handle interrupt from SysTick timer
 * @return	Nothing
 */

void SysTick_Handlerxxx(void)
{
	static uint8_t count = 0;

	/* Set output value on port 0 pins 0-3 (0-1 on the 824). Bits that are
	   not enabled by the mask are ignored when setting the port value */
	Chip_GPIO_PortSetMaskedState(LPC_GPIO_PORT, 0, count);

	Board_LED_Toggle(0);
	count++;
}

/**
 * @brief	Main program body
 * @return	Does not return
 */
int main(void) {

	/* Generic Initialization */
	SystemCoreClockUpdate();
	Board_Init();

	#if defined(BOARD_NXP_LPCXPRESSO_824)
	/* Enable clock to switch matrix so we can configure the matrix */
	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_SWM);
	/* 824 needs the fixed pin ACMP2 pin disabled to use pin as gpio */
	Chip_SWM_DisableFixedPin(SWM_FIXED_CLKIN);
	/* Turn clock to switch matrix back off to save power */
	Chip_Clock_DisablePeriphClock(SYSCTL_CLOCK_SWM);
	#endif

	/* Set port 0 pins 0-3 (0-1 on 824) to the output direction*/
	Chip_GPIO_PortSetDIR(LPC_GPIO_PORT, 0, PORT_MASK, 1);

	/* Set GPIO port mask value to make sure only port 0
	   pins 0-3 (0-1 on 824) are active during state change */
	Chip_GPIO_PortSetMask(LPC_GPIO_PORT, 0, ~PORT_MASK);

	/* Enable SysTick Timer */
	//SysTick_Config(SystemCoreClock / TICKRATE_HZ);


	Board_UARTPutSTR("START DEBUG seuos\r\n");
	
	
	/******************************************************/
	OSInit();
	OSTaskCreate(task0, &TaskStack[99], 1);
	OSTaskCreate(task1, &TaskStack1[99], 2);
	OSTaskCreate(task2, &TaskStack2[99], 3);
	OSStartTask();
	

	/* All work happens in the systick interrupt handler */
	while (1) {
		__WFI();
	}
}
