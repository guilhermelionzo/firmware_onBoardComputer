/*
 * ttcTask.c
 *
 *  Created on: 11 de jun de 2019
 *      Author: Guilherme
 */


#include "ttcTask.h"

void *ttcTask(void *pvParameters){

    while (1)
    {
        //TODO: the TT&C task
        (flag_lowBattery) ? vTaskDelay(TTC_TASK_TICK_PERIOD_LOW_BATTERY): vTaskDelay(TTC_TASK_TICK_PERIOD);

    }
}
