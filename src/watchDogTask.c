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

    while (1)
    {

        (flag_lowBattery) ?
                 vTaskDelayUntil(&xLastWakeTimeWatchDog,WATCHDOG_TASK_TICK_PERIOD_LOW_BATTERY) :
                 vTaskDelayUntil(&xLastWakeTimeWatchDog,WATCHDOG_TASK_TICK_PERIOD);            //


        uint32_t result = xEventGroupWaitBits(WATCHDOG_EVENT_GROUP,
                                              ALL_TASK_IDS, pdTRUE, pdTRUE, 1);

        if (!((result & ALL_TASK_IDS) == ALL_TASK_IDS))
        {
            GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
            //SysCtl_rebootDevice();
        }else{
            GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
        }


        //TODO: the AODCS task

    }
}

