/*
 * ttcTask.c
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
 * \file ttcTask.c
 *
 * \brief ttcTask file
 *
 * \author Guilherme Lionço
 *
 */


#include "ttcTask.h"


void setWatchDogBit_TTC(void);

void *ttcTask(void *pvParameters){


    /* The xLastWakeTime variable needs to be initialized with the current tick
     count. Note that this is the only time the variable is written to explicitly.
     After this xLastWakeTime is updated automatically internally within
     vTaskDelayUntil(). */
    portTickType xLastWakeTimeTTC = xTaskGetTickCount();


    while (1)
    {


        //DEBUG SESSION
        #if DEBUG_SESSION
        MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN2); // LED BLUE
        #endif

        uint8_t ttcComand;
        ttcComand = MAP_GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4);

        if(ttcComand==GPIO_INPUT_PIN_LOW){
            //don't set the bit in watchdog bits
        }else{
            setWatchDogBit_TTC();
        }

        vTaskDelay(10);

        //DEBUG SESSION
        #if DEBUG_SESSION
        //GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN2); // LED BLUE
        #endif


        //TODO: the TT&C task
       (flag_lowBattery) ?
                vTaskDelayUntil(&xLastWakeTimeTTC,TTC_TASK_TICK_PERIOD_LOW_BATTERY) :
                vTaskDelayUntil(&xLastWakeTimeTTC,TTC_TASK_TICK_PERIOD);            //
    }
}

void setWatchDogBit_TTC(void){

    xEventGroupSetBits(WATCHDOG_EVENT_GROUP, TTC_TASK_ID);

}
