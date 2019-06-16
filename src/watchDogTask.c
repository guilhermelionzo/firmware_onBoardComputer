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

void *watchDogTask(void *pvParameters)
{

    /* The xLastWakeTime variable needs to be initialized with the current tick
     count. Note that this is the only time the variable is written to explicitly.
     After this xLastWakeTime is updated automatically internally within
     vTaskDelayUntil(). */
    portTickType xLastWakeTimeWatchDog = xTaskGetTickCount();

    //initialize the MSP432 watchdog
    WDT_A_startTimer();


    while (1)
    {

        (flag_lowBattery) ?
                 vTaskDelayUntil(&xLastWakeTimeWatchDog,WATCHDOG_TASK_TICK_PERIOD_LOW_BATTERY) :
                 vTaskDelayUntil(&xLastWakeTimeWatchDog,WATCHDOG_TASK_TICK_PERIOD);            //


        uint32_t result = xEventGroupWaitBits(WATCHDOG_EVENT_GROUP,
                                              ALL_TASK_IDS, pdTRUE, pdTRUE, 1);

        //check whether all the task set the bits
        if (!((result & ALL_TASK_IDS) == ALL_TASK_IDS))
        {
            //TODO: try to reniciate the task that had delay
            GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
            //SysCtl_rebootDevice();
        }else{

            GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
        }

        //reset the MSP432 WatchDog
        WDT_A_clearTimer();

    }
}

