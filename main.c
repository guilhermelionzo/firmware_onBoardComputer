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
 * along with Firmware_OBC.  If not, see <http://www.gnu.org/licenses/>.
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
 *  extern functions (tasks)
 */
extern void *taskManager(void *pvParameters);



/*
 *  main functions
 */
static void prvSetupHardware(void);
static void prvConfigureClocks(void);
void prvCalendarConfiguration(void);
void configSWOPins(void);
void prvReadDataCounter(void);
void prvTaskCreat(void);
void prvQueueDataCreate();
void prvWatchDogEventGroupCreate(void);

/*-----------------------------------------------------------*/

/* The queue used by both tasks. */

/* Variables used to read data from flash memory*/

#define CALIBRATION_START 0x0003F000

/* Statics */
uint8_t obcFlashMemory;

/* pragmas to define the variables in flash */
#pragma DATA_SECTION(varOBCFlash, ".mydata");
#pragma DATA_ALIGN(varOBCFlash, 2);

uint8_t varOBCFlash;

int main(void)
{

    /* Prepare the hardware */
    prvSetupHardware();

    prvConfigureClocks();

    //configSWOPins();

    //chose between TRC_START_AWAIT_HOST or TRC_START
    vTraceEnable(TRC_START);

#if DEGUB_SESSION_TRACEALYZER
    stateChanel= xTraceRegisterString("STATE");
    vTracePrintF(stateChanel,"init");
    vTracePrintF(stateChanel,"hardware setup");
    vTracePrintF(stateChanel,"clock setup");
#endif

    /*init the calendar*/
    prvCalendarConfiguration();

#if DEGUB_SESSION_TRACEALYZER
    vTracePrintF(stateChanel,"create taskManager");
#endif
    //create the Task Manager task
    xTaskCreate((TaskFunction_t)taskManager, "Task Manager", configMINIMAL_STACK_SIZE, NULL, 5, NULL );

    /*read the previous counter*/
    //prvReadDataCounter();

#if DEGUB_SESSION_TRACEALYZER
    vTracePrintF(stateChanel,"start scheduler");
#endif

    /* Start the tasks and timer running. */
    vTaskStartScheduler();

    while (1)
    {

    }

}
/*-----------------------------------------------------------*/



void configSWOPins()
{

    /*SELECT P1.2 AND P1.3 IN UART MODE*/
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
    GPIO_PIN2 | GPIO_PIN3,GPIO_PRIMARY_MODULE_FUNCTION);
}

static void prvSetupHardware(void)
{
    extern void FPU_enableModule(void);

    /* The clocks are not configured here, but inside main_full() and
     main_blinky() as the full demo uses a fast clock and the blinky demo uses
     a slow clock. */

    /* Stop the watchdog timer. */
    MAP_WDT_A_holdTimer();

    /* Configuring WDT in interval mode to trigger every 8.192MHz clock iterations.
     * This comes out to roughly every 5.8 seconds */
    MAP_WDT_A_initIntervalTimer(WDT_A_CLOCKSOURCE_SMCLK,
                                WDT_A_CLOCKITERATIONS_512);

    MAP_WDT_A_setTimeoutReset(WDT_A_HARD_RESET);

    /* Ensure the FPU is enabled. */
    MAP_FPU_enableModule();

    //selecting P1.0(RED->WDT reset)
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);

    /* Configuring P1.4(receiving a TT&C comand) as an input and enabling interrupts */
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN4);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN4);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN4);
    //MAP_Interrupt_enableInterrupt(INT_PORT4);

    /* Selecting P2.0(RED->LOW BATTERY), P2.1(GREEN->DATA STORAGE) and P2.2 (BLUE->TT&C)*/
    MAP_GPIO_setAsOutputPin( GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2);

    flag_systemMode = NM_MODE;
    flag_lowBattery = BATTERY_LEVEL_5;
    configWakeUpPin();
}
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook(void)
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
    for (;;);
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook(void)
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

    /* Enabling Interrupts */
    //Interrupt_enableInterrupt(INT_ADC14);
    //Interrupt_enableMaster();

    MAP_PCM_gotoLPM0();
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
    (void) pcTaskName;
    (void) pxTask;

    /* Run time stack overflow checking is performed if
     configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
     function is called if a stack overflow is detected. */
    taskDISABLE_INTERRUPTS()
    ;
    for (;;)
        ;
}
/*-----------------------------------------------------------*/

void *malloc(size_t xSize)
{
    /* There should not be a heap defined, so trap any attempts to call
     malloc. */
    MAP_Interrupt_disableMaster();
    for (;;)
        ;
}
/*-----------------------------------------------------------*/

static void prvConfigureClocks(void)
{
    /* The full demo configures the clocks for maximum frequency, wheras this
     blinky demo uses a slower clock as it also uses low power features.

     From the datasheet:  For AM_LDO_VCORE0 and AM_DCDC_VCORE0 modes, the maximum
     CPU operating frequency is 24 MHz and maximum input clock frequency for
     peripherals is 12 MHz. */

    /* Set the core voltage level to VCORE1 */
    MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);
    MAP_Interrupt_disableMaster();

    /* Set 2 flash wait states for Flash bank 0 and 1*/
    MAP_FlashCtl_setWaitState( FLASH_BANK0, 2);
    MAP_FlashCtl_setWaitState( FLASH_BANK1, 2);

    /* Initializes Clock System */
    MAP_CS_setDCOCenteredFrequency( CS_DCO_FREQUENCY_48);                    //48MHZ
    MAP_CS_initClockSignal( CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    MAP_CS_initClockSignal( CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    MAP_CS_initClockSignal( CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    MAP_CS_initClockSignal( CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    /* The lower frequency allows the use of CVORE level 0. */
    //PCM_setCoreVoltageLevel( PCM_VCORE0 );
}

/*TODO: configure calendar with some trigger on TTC and storage the information on Flash memory'*/
void prvCalendarConfiguration(void)
{

    /*calendarConfig.seconds = 0;
    calendarConfig.minutes = 0;
    calendarConfig.hours = 0;
    calendarConfig.dayOfWeek = 0;
    calendarConfig.dayOfmonth = 11;
    calendarConfig.month = 6;
    calendarConfig.year = 0;*/

    //setup the calendar
    MAP_RTC_C_initCalendar(&calendarConfig, RTC_C_FORMAT_BINARY);

    MAP_RTC_C_startClock();
}

/*this function will read the data from msp432 flash memory (BANK1 SECTOR31)*/
void prvReadDataCounter(void)
{
    //eraseMemory();
    char data = "o";

    //![FlashCtl Program]
    /* Unprotecting Info Bank 0, Sector 0  */
    MAP_FlashCtl_unprotectSector(FLASH_MAIN_MEMORY_SPACE_BANK1, FLASH_SECTOR31);

    if (!MAP_FlashCtl_eraseSector(CALIBRATION_START))
        while (1)
            ;

    /* Trying to program the memory. Within this function, the API will
     automatically try to program the maximum number of tries. If it fails,
     trap inside an infinite loop */
    if (!MAP_FlashCtl_programMemory((void*) data,
                                    (void*) CALIBRATION_START, 1))
        while (1)
            ;
    /* Setting the sector back to protected  */
    MAP_FlashCtl_protectSector(FLASH_MAIN_MEMORY_SPACE_BANK1, FLASH_SECTOR31);
    //![FlashCtl Program]

    char *ptr;

    ptr =  (char *)CALIBRATION_START;

    data = *ptr;

    uint16_t tempValue;
    tempValue = *ptr;

    if (varOBCFlash == 0x100)
    {
        tempValue = varOBCFlash - 0x100;
    }
    tempValue = varOBCFlash;

    dataFlash.varReset = (tempValue & 0xFF);       // get the first 16bits
    dataFlash.varCounter = (tempValue >> 8) & 0xFF; // get the last 16bits (SHIFT 16BITS THEN MASK)
    dataFlash.varCounter++;
    dataFlash.varReset++;                             // increment varReset

    /* Since this program has a huge buffer that simulates the calibration data,
     * halting the watch dog is done in the reset ISR to avoid a watchdog
     * timeout during the zero
     */

    /* Initializing our buffer to a pattern of 0x00 */
    memset(obcFlashMemory, 0x00, sizeof(uint16_t));

    obcFlashMemory = dataFlash.varCounter;
    obcFlashMemory = (obcFlashMemory << 8);                           //&0xFFFF;
    obcFlashMemory |= dataFlash.varReset;
    //obcFlashMemory=0;

    /* Unprotecting Info Bank 0, Sector 0  */
    MAP_FlashCtl_unprotectSector(FLASH_MAIN_MEMORY_SPACE_BANK1, FLASH_SECTOR31);

    /* Trying to erase the sector. Within this function, the API will
     automatically try to erase the maximum number of tries. If it fails,
     trap in an infinite loop */
    if (!MAP_FlashCtl_eraseSector(CALIBRATION_START))
        while (1)
            ;
    /* Trying to program the memory. Within this function, the API will
     automatically try to program the maximum number of tries. If it fails,
     trap inside an infinite loop */
    if (!MAP_FlashCtl_programMemory((void*) obcFlashMemory,
                                    (void*) CALIBRATION_START, 2))
        while (1)
            ;
    /* Setting the sector back to protected  */
    MAP_FlashCtl_protectSector(FLASH_MAIN_MEMORY_SPACE_BANK1, FLASH_SECTOR31);

}

void eraseMemory()
{

    uint16_t temp;
    /* Initializing our buffer to a pattern of 0x00 */
    //memset(obcFlashMemory, 0x00, sizeof(uint32_t));
    /* Unprotecting Info Bank 0, Sector 0  */
    MAP_FlashCtl_unprotectSector(FLASH_MAIN_MEMORY_SPACE_BANK1, FLASH_SECTOR31);

    /* Trying to erase the sector. Within this function, the API will
     automatically try to erase the maximum number of tries. If it fails,
     trap in an infinite loop */
    if (!MAP_FlashCtl_eraseSector(CALIBRATION_START))
        while (1)
            ;

    /* Trying to program the memory. Within this function, the API will
     automatically try to program the maximum number of tries. If it fails,
     trap inside an infinite loop */
    if (!MAP_FlashCtl_programMemory(temp, (void*) CALIBRATION_START,
                                    sizeof(temp)))
        while (1)
            ;
    /* Setting the sector back to protected  */
    MAP_FlashCtl_protectSector(FLASH_MAIN_MEMORY_SPACE_BANK1, FLASH_SECTOR31);

}
void configWakeUpPin(){
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);
    MAP_Interrupt_disableSleepOnIsrExit();
}

void vPreSleepProcessing( uint32_t ulExpectedIdleTime )
{//CPU_wfi();

    //disable leds
    if(flag_systemMode==HM_MODE||flag_systemMode==BATTERY_LEVEL_1){
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2);
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0 );

    }

    MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);
    MAP_PCM_setPowerMode(PCM_DCDC_MODE);
    MAP_PCM_setPowerState(PCM_AM_DCDC_VCORE1);
    MAP_FPU_enableLazyStacking();

}
void vPostSleepProcessing( uint32_t ulExpectedIdleTime ){

    MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);
    MAP_PCM_setPowerMode(PCM_LDO_MODE);
    MAP_PCM_setPowerState(PCM_AM_LDO_VCORE1);
    MAP_CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
    MAP_FPU_enableStacking();

}
void PORT1_IRQHandler(void)
{
    uint32_t status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, status);

    if (status & GPIO_PIN1)
    {
        //clock=CS_getMCLK();
        MAP_Interrupt_disableInterrupt(INT_PORT1);
        //wakeState=1;
        MAP_PCM_setPowerState(PCM_AM_LDO_VCORE1);
        //clock=CS_getMCLK();

    }
}
