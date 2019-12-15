/*
 * watchDogTask.c
 *
 * Copyright (C) 2019, Universidade de Brasilia - FGA
 *
 * This file is part of Firmware_OBC.
 *
 * Firmware_OBC is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Firmware_OBC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Firmware_OBC.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * \file watchDogTask.c
 *
 * \brief watchDogTask file
 *
 * \author Guilherme Lionço
 *
 */

#include "watchDogTask.h"

int contDelete = 0;

extern QueueHandle_t xQueueDeletedTasks;

void *watchDogTask(void *pvParameters)
{

    /* The xLastWakeTime variable needs to be initialized with the current tick
     count. Note that this is the only time the variable is written to explicitly.
     After this xLastWakeTime is updated automatically internally within
     vTaskDelayUntil(). */
    portTickType xLastWakeTimeWatchDog = xTaskGetTickCount();

    //initialize the MSP432 watchdog
    MAP_WDT_A_startTimer();

    while (1)
    {

        (flag_lowBattery) ?
                vTaskDelayUntil(&xLastWakeTimeWatchDog,
                                WATCHDOG_TASK_TICK_PERIOD_LOW_BATTERY) :
                vTaskDelayUntil(&xLastWakeTimeWatchDog,
                                WATCHDOG_TASK_TICK_PERIOD);            //

        uint32_t result = xEventGroupWaitBits(WATCHDOG_EVENT_GROUP,
        ALL_TASK_IDS, pdTRUE, pdTRUE, 1);

        //check whether all the task set the bits
        if (!((result & ALL_TASK_IDS) == ALL_TASK_IDS))
        {
            deleteDelayedTask(result);

            //TODO: try to restart the task that had delay
            MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
            //SysCtl_rebootDevice();
        }
        else
        {

            MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
        }
        xEventGroupClearBits(WATCHDOG_EVENT_GROUP, ALL_TASK_IDS);

        //reset the MSP432 WatchDog


        if (contDelete < 30)
        {
            MAP_WDT_A_clearTimer();
            //vTaskDelete((TaskFunction_t) taskHandlerTTC);


        }
        if (contDelete == 32)
        {
            //taskHandlerTTC = NULL;
            //deleted = 1;
        }
        contDelete++;
    }
}

//TODO: code clean up. Think in a better way to arrange this logic
void deleteDelayedTask(uint32_t result)
{

   if (result & AODCS_TASK_ID == AODCS_TASK_ID)
    {

        vTaskDelete(taskHandlerAodcs);
        taskHandlerAodcs=NULL;
        //send the taskHandler to queue
        while (xQueueSend(xQueueDeletedTasks, 1, 0)!=pdPASS);
    }
    if (result & CAMERA_TASK_ID == CAMERA_TASK_ID)
    {
        vTaskDelete(taskHandlerCamera);
        taskHandlerCamera=NULL;
        //send the taskHandler to queue
        while (xQueueSend(xQueueDeletedTasks,2, 0)!=pdPASS);
    }
    if (result & DATASTORAGE_TASK_ID == DATASTORAGE_TASK_ID)
    {
        vTaskDelete(taskHandlerDataStorage);
        taskHandlerDataStorage=NULL;
        //send the taskHandler to queue
        while (xQueueSend(xQueueDeletedTasks, 3, 0)!=pdPASS);
    }
    if (result & HOUSEKEEPING_TASK_ID == HOUSEKEEPING_TASK_ID)
    {
        vTaskDelete(taskHandlerHouseKeeping);
        taskHandlerHouseKeeping=NULL;
        //send the taskHandler to queue
        while (xQueueSend(xQueueDeletedTasks, 4, 0)!=pdPASS);
    }
    if (result & PPT_TASK_ID == PPT_TASK_ID)
    {
        vTaskDelete(taskHandlerPPT);
        taskHandlerPPT=NULL;
        //send the taskHandler to queue
        while (xQueueSend(xQueueDeletedTasks, 5, 0)!=pdPASS);
    }
    if ((result & TTC_TASK_ID) != TTC_TASK_ID)
    {
        vTaskDelete(taskHandlerTTC);
        taskHandlerTTC=NULL;
        //send the taskHandler to queue
        while (xQueueSend(xQueueDeletedTasks, 6, 0)!=pdPASS);
    }
}
