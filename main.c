/*
 * main.c
 *
 * Copyright (C) 2019, Universidade de Brasilia - FGA
 *
 * This file is part of Firmware_OBC.
 *
 * Firmware_OBC is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Firmware_OBC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with FloripaSat-OBDH.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

 /**
 * \file main.c
 *
 * \brief Main file
 *
 * \author Guilherme Lionço
 *
 */

/* Standard includes. */
#include <stdio.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/*-----------------------------------------------------------*/

/*
 * Set up the hardware ready to run this demo.
 */

static void prvSetupHardware( void );

/*
 * main_blinky() is used when configCREATE_SIMPLE_TICKLESS_DEMO is set to 1.
 * main_full() is used when configCREATE_SIMPLE_TICKLESS_DEMO is set to 0.
 */
extern void main_blinky( void );
extern void dataStorage(void);
/*-----------------------------------------------------------*/

int main( void )
{
	/* See http://www.FreeRTOS.org/TI_MSP432_Free_RTOS_Demo.html for instructions. */

	/* Prepare the hardware to run this demo. */
	prvSetupHardware();

	//main_blinky();
	vTraceEnable(TRC_START);
	xTaskCreate(dataStorage, "Data Storage", 1024, NULL, 1, NULL);
    /* Start the tasks and timer running. */

	vTaskStartScheduler();

	while(1);

}
/*-----------------------------------------------------------*/

static void prvSetupHardware( void )
{
extern void FPU_enableModule( void );

	/* The clocks are not configured here, but inside main_full() and
	main_blinky() as the full demo uses a fast clock and the blinky demo uses
	a slow clock. */

	/* Stop the watchdog timer. */
	MAP_WDT_A_holdTimer();

	/* Ensure the FPU is enabled. */
	FPU_enableModule();

	/* Selecting P1.2 and P1.3 in UART mode and P1.0 as output (LED) */
	MAP_GPIO_setAsPeripheralModuleFunctionInputPin( GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION );
	MAP_GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN0 );
	MAP_GPIO_setAsOutputPin( GPIO_PORT_P1, GPIO_PIN0 );
}
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c or heap_2.c are used, then the size of the
	heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains (although it does not
	provide information on how the remaining heap might be fragmented). */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	task.  It is essential that code added to this hook function never attempts
	to block in any way (for example, call xQueueReceive() with a block time
	specified, or call vTaskDelay()).  If the application makes use of the
	vTaskDelete() API function (as this demo application does) then it is also
	important that vApplicationIdleHook() is permitted to return to its calling
	function, because it is the responsibility of the idle task to clean up
	memory allocated by the kernel to any task that has since been deleted. */
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/

void *malloc( size_t xSize )
{
	/* There should not be a heap defined, so trap any attempts to call
	malloc. */
	Interrupt_disableMaster();
	for( ;; );
}
/*-----------------------------------------------------------*/


