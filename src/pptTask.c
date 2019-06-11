/*
 * pptTask.c
 *
 *  Created on: 11 de jun de 2019
 *      Author: Guilherme
 */


#include "pptTask.h"

void *pptTask(void *pvParameters){

    while (1)
    {
        //TODO: the PPT task
        (flag_lowBattery) ? vTaskDelay(PPT_TASK_TICK_PERIOD_LOW_BATTERY): vTaskDelay(PPT_TASK_TICK_PERIOD);

    }
}


