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
void setOperationMode(uint8_t op_mode);
void killAllTasks(void);
void prvTaskCreate(void);
void prvWatchDogEventGroupCreate(void);
void prvQueueDataCreate(void);
void prvQueueCommunicationCreate(void);

extern void *aodcsTask(void *pvParameters);
extern void *cameraTask(void *pvParameters);
extern void *dataStorage(void *pvParameters);
extern void *houseKeeping(void *pvParameters);
extern void *pptTask(void *pvParameters);
extern void *ttcTask(void *pvParameters);
extern void *watchDogTask(void *pvParameters);
extern void *sensorTask(void *pvParameters);


/***** HANDLES *****/
QueueHandle_t xQueueIMU=NULL;
QueueHandle_t xQueueDataObc=NULL;
QueueHandle_t xQueueSystem=NULL;
SemaphoreHandle_t semaphoreIMU=NULL;
SemaphoreHandle_t semaphoreSystem=NULL;

uint16_t batteryValue=0;

void *taskManager(void *pvParameters)
{
    memset(obcData, 0x00, sizeof(obcData));

    memcpy(obcData.system_status, "NM", sizeof("NM"));

    prvTaskCreate();

    portTickType xLastWakeTimeTaskManager = xTaskGetTickCount();

    while (1)
    {

        updateBatteryLevel();

        //xQueueReceive(xQueueSystem, &batteryValue, 2000);

        //setOperationMode(batteryValue);

        (flag_lowBattery) ?
                vTaskDelayUntil(&xLastWakeTimeTaskManager,
                                TASK_MANAGER_TICK_PERIOD_LOW_BATTERY) :
                vTaskDelayUntil(&xLastWakeTimeTaskManager,
                                TASK_MANAGER_TICK_PERIOD);
    }

}

void prvTaskCreate(void){

    /*create queue to exchange data between the tasks*/
    prvQueueDataCreate();

    prvWatchDogEventGroupCreate();

    xTaskCreate((TaskFunction_t)aodcsTask   , "AODCS Task"   , 1024, NULL, 1, NULL);
    xTaskCreate((TaskFunction_t)cameraTask  , "CAMERA Task"  , 1024, NULL, 1, NULL);
    xTaskCreate((TaskFunction_t)houseKeeping, "House Keeping", 2*1024, NULL, 2, NULL);
    xTaskCreate((TaskFunction_t)dataStorage , "Data Storage" , 1024, NULL, 1, NULL);
    xTaskCreate((TaskFunction_t)pptTask     , "PPT Task"     , 1024, NULL, 1, NULL);
    xTaskCreate((TaskFunction_t)ttcTask     , "TT&C Task"    , 1024, NULL, 1, NULL);
    xTaskCreate((TaskFunction_t)watchDogTask, "WTD Task"     , 1024, NULL, 5, NULL);

    //xTaskCreate(sensorTask, "SS", 1024, NULL, 1, NULL);

}

void setOperationMode(uint8_t op_mode)
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

    xQueueReceive(xQueueSystem, &batteryValue, 3000);

    //BATERY LEVEL 1
    if (batteryValue >= 5000 && batteryValue < 12500)
    {

        //DEBUG SESSION
#if DEBUG_SESSION
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0); // PIN RED
#endif

        memcpy(obcData.system_status, "BLLM1", sizeof("BLLM1"));
        flag_lowBattery = BATTERY_LEVEL_1;

    }
    //HIBERNATE MODE
    else if (batteryValue < 5000){

        //DEBUG SESSION
#if DEBUG_SESSION
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0); // PIN RED
#endif
        memcpy(obcData.system_status, "HM_MODE", sizeof("HM_MODE"));
        flag_lowBattery = BATTERY_LEVEL_0;
    }
    //NORMAL MODE
    else{

        //DEBUG SESSION
#if DEBUG_SESSION
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0); // PIN RED
#endif
        memcpy(obcData.system_status, "NM_MODE", sizeof("NM_MODE"));
        flag_lowBattery = BATTERY_LEVEL_5;

    }
}

void prvWatchDogEventGroupCreate(void){

    //create the event group for wtd
    WATCHDOG_EVENT_GROUP = xEventGroupCreate();
}

/* CREATE THE QUEUES TO SUPPORT THE DATA EXCHANGE BETWEEN THE TASKS*/
void prvQueueDataCreate(void){

    //creating a Queue to handle the data between the tasks
    xQueueIMU = xQueueCreate( 9,SIZE_OF_IMU_DATA*8);
    //xQueueDataObc=xQueueCreate( 1,sizeof(obcData));
    //xQueueIMU = xQueueCreate( 1,sizeof(obcData));
    xQueueSystem =xQueueCreate( 1,sizeof(int16_t));
    semaphoreSystem = xSemaphoreCreateMutex();

    semaphoreIMU = xSemaphoreCreateMutex();

}

void prvQueueCommunicationCreate(void){


}


#endif /* SRC_TASKMANAGER_C_ */
