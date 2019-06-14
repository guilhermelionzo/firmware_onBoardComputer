/*
 * pptTask.c
 *
 *  Created on: 11 de jun de 2019
 *      Author: Guilherme
 */


#include "pptTask.h"

void setWatchDogBit_PPT(void);

void *pptTask(void *pvParameters){


    /* The xLastWakeTime variable needs to be initialized with the current tick
     count. Note that this is the only time the variable is written to explicitly.
     After this xLastWakeTime is updated automatically internally within
     vTaskDelayUntil(). */
    portTickType xLastWakeTimePPT = xTaskGetTickCount();


    while (1)
    {
        //TODO: the PPT task

        setWatchDogBit_PPT();

        (flag_lowBattery) ?
                vTaskDelayUntil(&xLastWakeTimePPT,PPT_TASK_TICK_PERIOD_LOW_BATTERY) :
                vTaskDelayUntil(&xLastWakeTimePPT,PPT_TASK_TICK_PERIOD);            //

    }
}

void setWatchDogBit_PPT(void){

    xEventGroupSetBits(WATCHDOG_EVENT_GROUP, PPT_TASK_ID);

}

