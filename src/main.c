/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/*-----------------------------------------------------------*/
/* User include files. */
#include "mavlink_receive.h"
#include "danger_check.h"
#include "ppm_encoder.h"
#include "mission.h"

/*-----------------------------------------------------------*/
/* Tool include files. */
#include "printf-stdarg.h"

/*-----------------------------------------------------------*/
/* define macros. */
#define INIT_TASK_PRI   1

/*-----------------------------------------------------------*/
/*
 * Configure the hardware as necessary to run this demo.
 */
static void prvSetupHardware( void );
static void init_task_entry(void *pvParameters);

/*-----------------------------------------------------------*/

void main(void)
{
	/* Configure the hardware ready to run the demo. */
	prvSetupHardware();

	xTaskCreate(init_task_entry,
	                      "init",
	                      configMINIMAL_STACK_SIZE * 2,
	                      NULL,
	                      INIT_TASK_PRI,
	                      NULL);

	vTaskStartScheduler();

	while(1);
}
/*-----------------------------------------------------------*/

static void prvSetupHardware( void )
{
uint16_t usProtectDummy = ( uint16_t ) ( SYSTEM.PRCR.WORD & 0x000FU );

	/* Disable protect bit */
	SYSTEM.PRCR.WORD = 0xA50FU;

	/* Restore the previous state of the protect register */
	SYSTEM.PRCR.WORD = ( uint16_t )( 0xA500U | usProtectDummy );
}
/*-----------------------------------------------------------*/

static void init_task_entry(void *pvParameters)
{
    mavlink_receive_init();
    ppm_encoder_init();
    danger_check_init();
    mission_init();
    debug_printf("\nInitialization is done!\n");
    /* When initialization is done ,this task can be deleted. */
    vTaskDelete(NULL);
}
/*-----------------------------------------------------------*/
