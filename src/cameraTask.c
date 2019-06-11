/*
 * cameraTask.c
 *
 *  Created on: 11 de jun de 2019
 *      Author: Guilherme
 */



#include "cameraTask.h"

void *cameraTask(void *pvParameters){

    while (1)
    {
        //TODO: the CAMERA task
        (flag_lowBattery) ? vTaskDelay(CAMERA_TASK_TICK_PERIOD_LOW_BATTERY): vTaskDelay(CAMERA_TASK_TICK_PERIOD);

    }
}

