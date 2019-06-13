/*
 * pptTask.c
 *
 *  Created on: 11 de jun de 2019
 *      Author: Guilherme
 */


#include "pptTask.h"

void setWatchDogBit_PPT(void);

void *pptTask(void *pvParameters){

    while (1)
    {
        //TODO: the PPT task

        setWatchDogBit_PPT();

        (flag_lowBattery) ? vTaskDelay(PPT_TASK_TICK_PERIOD_LOW_BATTERY): vTaskDelay(PPT_TASK_TICK_PERIOD);

    }
}

void setWatchDogBit_PPT(void){

    xEventGroupSetBits(WATCHDOG_EVENT_GROUP, PPT_TASK_ID);

}

