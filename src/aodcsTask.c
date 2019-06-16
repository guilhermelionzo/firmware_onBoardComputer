/*
 * aodcsTask.c
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
 * \file aodcsTask.c
 *
 * \brief aodcsTask file
 *
 * \author Guilherme Lionço
 *
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
