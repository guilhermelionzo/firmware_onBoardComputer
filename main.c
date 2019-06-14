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

/*-----------------------------------------------------------*/

#include <src/systemDef.h>
#include <src/taskManager.h>

/*
 * Set up the hardware ready to run this demo.
 */

static void prvSetupHardware( void );
static void prvConfigureClocks( void );
void calendarConfiguration(void);
void configSWOPins(void);
/*
 * main_blinky() is used when configCREATE_SIMPLE_TICKLESS_DEMO is set to 1.
 * main_full() is used when configCREATE_SIMPLE_TICKLESS_DEMO is set to 0.
 */

/*-----------------------------------------------------------*/

/* The queue used by both tasks. */


int main( void )
{

	/* Prepare the hardware */
	prvSetupHardware();

	prvConfigureClocks();

	//main_blinky();

	//configSWOPins();

	//chose between TRC_START_AWAIT_HOST or TRC_START
	vTraceEnable(TRC_START);

	/*init the calendar*/
	//calendarConfiguration();

	/*Start the creation of tasks*/
	taskCreate();
	flag_lowBattery=0;

	/* Start the tasks and timer running. */
	vTaskStartScheduler();

	while(1){


	}

}
/*-----------------------------------------------------------*/

void configSWOPins(){

    /*SELECT P1.2 AND P1.3 IN UART MODE*/
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN2|GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
}

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

	//selecting P1.0(RED->WDT reset)
	GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);

    /* Configuring P1.4(receiving a TT&C comand) as an input and enabling interrupts */
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN4);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN4);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN4);
    //MAP_Interrupt_enableInterrupt(INT_PORT4);

	/* Selecting P2.0(RED->LOW BATTERY), P2.1(GREEN->DATA STORAGE) and P2.2 (BLUE->TT&C)*/
	MAP_GPIO_setAsOutputPin( GPIO_PORT_P2, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
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

static void prvConfigureClocks( void )
{
    /* The full demo configures the clocks for maximum frequency, wheras this
    blinky demo uses a slower clock as it also uses low power features.

    From the datasheet:  For AM_LDO_VCORE0 and AM_DCDC_VCORE0 modes, the maximum
    CPU operating frequency is 24 MHz and maximum input clock frequency for
    peripherals is 12 MHz. */

    /* Set the core voltage level to VCORE1 */
    PCM_setCoreVoltageLevel(PCM_VCORE1);
    MAP_Interrupt_disableMaster();

    /* Set 2 flash wait states for Flash bank 0 and 1*/
    FlashCtl_setWaitState( FLASH_BANK0, 2 );
    FlashCtl_setWaitState( FLASH_BANK1, 2 );

    /* Initializes Clock System */
    CS_setDCOCenteredFrequency( CS_DCO_FREQUENCY_48);                       //48MHZ
    CS_initClockSignal( CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1 );
    CS_initClockSignal( CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1 );
    CS_initClockSignal( CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1 );
    CS_initClockSignal( CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1 );

    /* The lower frequency allows the use of CVORE level 0. */
    //PCM_setCoreVoltageLevel( PCM_VCORE0 );
}

/*TODO: configure calendar with some trigger on TTC and storage the information on Flash memory'*/
void calendarConfiguration(void){

   calendarConfig.seconds = 0;
   calendarConfig.minutes = 0;
   calendarConfig.hours = 0;
   calendarConfig.dayOfWeek = 0;
   calendarConfig.dayOfmonth = 11;
   calendarConfig.month = 6;
   calendarConfig.year = 0;

   //setup the calendar
   RTC_C_initCalendar(&calendarConfig,RTC_C_FORMAT_BINARY);

   RTC_C_startClock();
}
