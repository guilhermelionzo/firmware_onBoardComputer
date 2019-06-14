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


    /* The xLastWakeTime variable needs to be initialized with the current tick
     count. Note that this is the only time the variable is written to explicitly.
     After this xLastWakeTime is updated automatically internally within
     vTaskDelayUntil(). */
    portTickType xLastWakeTimeAODCS = xTaskGetTickCount();

    while (1)
    {
        //TODO: the AODCS task
        setWatchDogBit_AODCS();

        (flag_lowBattery) ?
                vTaskDelayUntil(&xLastWakeTimeAODCS,AODCS_TASK_TICK_PERIOD_LOW_BATTERY) :
                vTaskDelayUntil(&xLastWakeTimeAODCS, AODCS_TASK_TICK_PERIOD);            //

    }
}

void setWatchDogBit_AODCS(void)
{

    xEventGroupSetBits(WATCHDOG_EVENT_GROUP, AODCS_TASK_ID);

}
