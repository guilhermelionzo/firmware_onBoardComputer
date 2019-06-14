/*
 * whatchDogTask.c
 *
 *  Created on: 13 de jun de 2019
 *      Author: Guilherme
 */

#include "watchDogTask.h"

void *watchDogTask(void *pvParameters)
{

    /* The xLastWakeTime variable needs to be initialized with the current tick
     count. Note that this is the only time the variable is written to explicitly.
     After this xLastWakeTime is updated automatically internally within
     vTaskDelayUntil(). */
    portTickType xLastWakeTimeWatchDog = xTaskGetTickCount();

    //initialize the MSP432 watchdog
    WDT_A_startTimer();


    while (1)
    {

        (flag_lowBattery) ?
                 vTaskDelayUntil(&xLastWakeTimeWatchDog,WATCHDOG_TASK_TICK_PERIOD_LOW_BATTERY) :
                 vTaskDelayUntil(&xLastWakeTimeWatchDog,WATCHDOG_TASK_TICK_PERIOD);            //


        uint32_t result = xEventGroupWaitBits(WATCHDOG_EVENT_GROUP,
                                              ALL_TASK_IDS, pdTRUE, pdTRUE, 1);

        //check whether all the task set the bits
        if (!((result & ALL_TASK_IDS) == ALL_TASK_IDS))
        {
            //TODO: try to reniciate the task that had delay
            GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
            //SysCtl_rebootDevice();
        }else{

            GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
        }

        //reset the MSP432 WatchDog
        WDT_A_clearTimer();

    }
}

