/*
 * whatchDogTask.c
 *
 *  Created on: 13 de jun de 2019
 *      Author: Guilherme
 */

#include "watchDogTask.h"

void *watchDogTask(void *pvParameters)
{

    while (1)
    {
        (flag_lowBattery) ? vTaskDelay(WATCHDOG_TASK_TICK_PERIOD_LOW_BATTERY): vTaskDelay(WATCHDOG_TASK_TICK_PERIOD);

        uint32_t result = xEventGroupWaitBits(WATCHDOG_EVENT_GROUP, ALL_TASK_IDS, pdTRUE, pdTRUE, 1);

        if((result & ALL_TASK_IDS) == ALL_TASK_IDS){
            GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN2);
        }else{
            GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN2);
            //SysCtl_rebootDevice();
        }

        //TODO: the AODCS task

    }
}



