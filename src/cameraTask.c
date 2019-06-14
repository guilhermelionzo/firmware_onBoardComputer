/*
 * cameraTask.c
 *
 *  Created on: 11 de jun de 2019
 *      Author: Guilherme
 */



#include "cameraTask.h"

void setWatchDogBit_CAMERA(void);

void *cameraTask(void *pvParameters){

    /* The xLastWakeTime variable needs to be initialized with the current tick
     count. Note that this is the only time the variable is written to explicitly.
     After this xLastWakeTime is updated automatically internally within
     vTaskDelayUntil(). */
    portTickType xLastWakeTimeCAMERA = xTaskGetTickCount();


    while (1)
    {
        setWatchDogBit_CAMERA();
        //TODO: the CAMERA task

        (flag_lowBattery) ?
                vTaskDelayUntil(&xLastWakeTimeCAMERA,CAMERA_TASK_TICK_PERIOD_LOW_BATTERY) :
                vTaskDelayUntil(&xLastWakeTimeCAMERA, CAMERA_TASK_TICK_PERIOD);            //

    }
}

void setWatchDogBit_CAMERA(void){

    xEventGroupSetBits(WATCHDOG_EVENT_GROUP, CAMERA_TASK_ID);

}
