/*
 * aodcsTask.c
 *
 *  Created on: 11 de jun de 2019
 *      Author: Guilherme
 */

#include "aodcsTask.h"

void setWatchDogBit_AODCS(void);

void *aodcsTask(void *pvParameters)
{

    while (1)
    {
        //TODO: the AODCS task
        setWatchDogBit_AODCS();
        (flag_lowBattery) ? vTaskDelay(AODCS_TASK_TICK_PERIOD_LOW_BATTERY): vTaskDelay(AODCS_TASK_TICK_PERIOD);

    }
}

void setWatchDogBit_AODCS(void){

    xEventGroupSetBits(WATCHDOG_EVENT_GROUP, AODCS_TASK_ID);

}
