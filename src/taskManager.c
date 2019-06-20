/*
 * taskManager.c
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
 * \file taskManager.c
 *
 * \brief taskManager file
 *
 * \author Guilherme Lionço
 *
 */

#ifndef SRC_TASKMANAGER_C_
#define SRC_TASKMANAGER_C_

#include "taskManager.h"

void updateBatteryLevel(void);
void operationMode(uint8_t op_mode);

float batteryValue;

void *taskManager(void *pvParameters)
{

    portTickType xLastWakeTimeTaskManager = xTaskGetTickCount();

    while (1)
    {

        updateBatteryLevel();

        xQueueReceive(xQueueIMU, &imuData, 200);

        operationMode(obcData.system_status[5]);

        (flag_lowBattery) ?
                vTaskDelayUntil(&xLastWakeTimeTaskManager,
                                TASK_MANAGER_TICK_PERIOD_LOW_BATTERY) :
                vTaskDelayUntil(&xLastWakeTimeTaskManager,
                                TASK_MANAGER_TICK_PERIOD);
    }

}

void operationMode(uint8_t op_mode)
{

    switch (op_mode)
    {
    case NM_MODE:

        PCM_setCoreVoltageLevel(PCM_VCORE1);
        PCM_setPowerMode(PCM_LDO_MODE);
        PCM_setPowerState(PCM_AM_LDO_VCORE1);

        break;
    case BLLM1_MODE:

        PCM_setCoreVoltageLevel(PCM_VCORE1);
        PCM_setPowerMode(PCM_DCDC_MODE);
        PCM_setPowerState(PCM_AM_DCDC_VCORE1);

        break;
    case HM_MODE:

        PCM_setCoreVoltageLevel(PCM_VCORE1);
        //PCM_setPowerMode(PCM_DCDC_MODE);
        PCM_setPowerState(PCM_LPM0_LF_VCORE1);

        killAllTasks();

        break;
    case SM_MODE:
        //TODO
        break;
    default:
        break;
    }

}

void killAllTasks(void)
{

    //TODO:

}

void updateBatteryLevel(void)
{

    xQueueReceive(xQueueSystem, &imuData, 200);

    if (obcData.obc_sensors[5] < 4000)
    {

        //DEBUG SESSION
#if DEBUG_SESSION
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0); // PIN RED
#endif
        obcData.system_status[5] = NM_MODE;
        flag_lowBattery = BATTERY_LEVEL_1;

    }
    else if (obcData.obc_sensors[5] < 7000)
    {

        //DEBUG SESSION
#if DEBUG_SESSION
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0); // PIN RED
#endif
        obcData.system_status[5] = HM_MODE;
        flag_lowBattery = BATTERY_LEVEL_0;
    }

}

#endif /* SRC_TASKMANAGER_C_ */
