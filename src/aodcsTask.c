/*
 * aodcsTask.c
 *
 *  Created on: 11 de jun de 2019
 *      Author: Guilherme
 */

#include "aodcsTask.h"

void *aodcsTask(void *pvParameters)
{

    while (1)
    {
        //TODO: the AODCS task
        (flag_lowBattery) ? vTaskDelay(AODCS_TASK_TICK_PERIOD_LOW_BATTERY): vTaskDelay(AODCS_TASK_TICK_PERIOD);

    }
}
