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
void setOperationMode(void);
void killAllTasks(void);
void prvTaskCreate(void);
void prvWatchDogEventGroupCreate(void);
void prvQueueDataCreate(void);
void prvQueueCommunicationCreate(void);
void prvSetClock();
void prvResumeAllTasks(void);
void prvSuspendAllTasks(void);
extern void *aodcsTask(void *pvParameters);
extern void *cameraTask(void *pvParameters);
extern void *dataStorage(void *pvParameters);
extern void *houseKeeping(void *pvParameters);
extern void *pptTask(void *pvParameters);
extern void *ttcTask(void *pvParameters);
extern void *watchDogTask(void *pvParameters);

/***** HANDLES *****/
QueueHandle_t xQueueIMU = NULL;
QueueHandle_t xQueueDataObc = NULL;
QueueHandle_t xQueueSystem = NULL;
SemaphoreHandle_t semaphoreIMU = NULL;
SemaphoreHandle_t semaphoreSystem = NULL;

TaskHandle_t taskHandlerAodcs;
TaskHandle_t taskHandlerCamera;
TaskHandle_t taskHandlerDataStorage;
TaskHandle_t taskHandlerHouseKeeping;
TaskHandle_t taskHandlerPpt;
TaskHandle_t taskHandlerTtc;
TaskHandle_t taskHandlerWatchDog;

uint16_t batteryValue = 0;

uint32_t clock = 0;
uint8_t adcInitDelay = 0;

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

        setOperationMode();

        (flag_lowBattery) ? vTaskDelayUntil(&xLastWakeTimeTaskManager,
        TASK_MANAGER_TICK_PERIOD_LOW_BATTERY) :
                            vTaskDelayUntil(&xLastWakeTimeTaskManager,
                            TASK_MANAGER_TICK_PERIOD);
    }

}

void prvTaskCreate(void)
{

    /*create queue to exchange data between the tasks*/
    prvQueueDataCreate();

    prvWatchDogEventGroupCreate();

    xTaskCreate((TaskFunction_t) aodcsTask, "AODCS Task", 1024, NULL, 1,
                &taskHandlerAodcs);
    xTaskCreate((TaskFunction_t) cameraTask, "CAMERA Task", 1024, NULL, 1,
                &taskHandlerCamera);
    xTaskCreate((TaskFunction_t) houseKeeping, "House Keeping",1024, NULL,
                2, &taskHandlerDataStorage);
    xTaskCreate((TaskFunction_t) dataStorage, "Data Storage",1024, NULL, 1,
                &taskHandlerHouseKeeping);
    xTaskCreate((TaskFunction_t) pptTask, "PPT Task", 1024, NULL, 1,
                &taskHandlerPpt);
    xTaskCreate((TaskFunction_t) ttcTask, "TT&C Task", 1024, NULL, 1,
                &taskHandlerTtc);
    xTaskCreate((TaskFunction_t) watchDogTask, "WTD Task", 500, NULL, 5,
                &taskHandlerWatchDog);

}

void setOperationMode(void)
{

    clock = CS_getMCLK();

    switch (flag_systemMode)
    {
    case NM_MODE:

        MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);
        MAP_PCM_setPowerMode(PCM_LDO_MODE);
        MAP_PCM_setPowerState(PCM_AM_LDO_VCORE1);
        //prvSetClock();
        prvResumeAllTasks();

        break;
    case BLLM1_MODE:

        MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);
        MAP_PCM_setPowerMode(PCM_DCDC_MODE);
        MAP_PCM_setPowerState(PCM_AM_DCDC_VCORE1);
        //prvSetClock();
        prvResumeAllTasks();

        break;
    case HM_MODE:

        MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);
        MAP_PCM_setPowerMode(PCM_DCDC_MODE);
        MAP_PCM_setPowerState(PCM_AM_DCDC_VCORE1);
        //prvSetClock();
        //vPortSuppressTicksAndSleep(30);

        prvSuspendAllTasks();

        //interrupt just for see the result of the battery level and update the state
        //vPortSuppressTicksAndSleep(50);

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

    if (adcInitDelay < 5)
    {
        adcInitDelay++;
        memcpy(obcData.system_status, "INIT", sizeof("INIT"));
        flag_systemMode = NM_MODE;
        flag_lowBattery = BATTERY_LEVEL_5;
        batteryValue = 0;

    }
    else
    {

        batteryValue = ADC14_getResult(ADC_MEM5);

        //BATERY LEVEL 1
        if (batteryValue >= 4000 && batteryValue < 12500)
        {

            //DEBUG SESSION
#if DEBUG_SESSION
            MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN0); // PIN RED
#endif

            memcpy(obcData.system_status, "BLLM1", sizeof("BLLM1"));
            flag_systemMode = BLLM1_MODE;
            flag_lowBattery = BATTERY_LEVEL_1;
            batteryValue = 1;

        }
        //HIBERNATE MODE
        else if (batteryValue < 4000)
        {

            //DEBUG SESSION
#if DEBUG_SESSION
            MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0); // PIN RED
#endif
            memcpy(obcData.system_status, "HM_MODE", sizeof("HM_MODE"));
            flag_systemMode = HM_MODE;
            flag_lowBattery = BATTERY_LEVEL_0;
            batteryValue = 1;
        }
        //NORMAL MODE
        else
        {

            //DEBUG SESSION
#if DEBUG_SESSION
            MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0); // PIN RED
#endif
            memcpy(obcData.system_status, "NM_MODE", sizeof("NM_MODE"));
            flag_systemMode = NM_MODE;
            flag_lowBattery = BATTERY_LEVEL_5;
            batteryValue = 0;

        }
    }
}

void prvWatchDogEventGroupCreate(void)
{

    //create the event group for wtd
    WATCHDOG_EVENT_GROUP = xEventGroupCreate();
}

/* CREATE THE QUEUES TO SUPPORT THE DATA EXCHANGE BETWEEN THE TASKS*/
void prvQueueDataCreate(void)
{

    //creating a Queue to handle the data between the tasks
    xQueueIMU = xQueueCreate(9, SIZE_OF_IMU_DATA*8);
    //xQueueDataObc=xQueueCreate( 1,sizeof(obcData));
    //xQueueIMU = xQueueCreate( 1,sizeof(obcData));
    xQueueSystem = xQueueCreate(1, sizeof(int16_t));
    semaphoreSystem = xSemaphoreCreateMutex();

    semaphoreIMU = xSemaphoreCreateMutex();

}

void prvQueueCommunicationCreate(void)
{

}

void prvSuspendAllTasks()
{

    vTaskSuspend(taskHandlerAodcs);
    vTaskSuspend(taskHandlerCamera);
    vTaskSuspend(taskHandlerDataStorage);
    vTaskSuspend(taskHandlerHouseKeeping);
    vTaskSuspend(taskHandlerPpt);
    vTaskSuspend(taskHandlerTtc);
    vTaskSuspend( taskHandlerWatchDog  );
    MAP_WDT_A_holdTimer();
}

void prvResumeAllTasks()
{

    vTaskResume(taskHandlerAodcs);
    vTaskResume(taskHandlerCamera);
    vTaskResume(taskHandlerDataStorage);
    vTaskResume(taskHandlerHouseKeeping);
    vTaskResume(taskHandlerPpt);
    vTaskResume(taskHandlerTtc);
    vTaskResume( taskHandlerWatchDog  );
    MAP_WDT_A_startTimer();

}

void prvSetClock()
{

    switch (flag_systemMode)
    {
    case NM_MODE:
        /* Set 2 flash wait states for Flash bank 0 and 1*/
        MAP_FlashCtl_setWaitState( FLASH_BANK0, 2);
        MAP_FlashCtl_setWaitState( FLASH_BANK1, 2);

        /* Initializes Clock System */
        MAP_CS_setDCOCenteredFrequency( CS_DCO_FREQUENCY_48);                //48MHZ
        MAP_CS_initClockSignal( CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
        break;

    case BLLM1_MODE:
        /* At 4MHz in VCORE0, MSP432P401R needs 0 wait states */
        //MAP_FlashCtl_setWaitState(FLASH_BANK0, 0);
        //MAP_FlashCtl_setWaitState(FLASH_BANK1, 0);
        /* Set DCO to 4MHz */
        MAP_CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_24);
        //MAP_CS_setDCOFrequency(2400000);

        /* Set MCLK = DCO = 4MHz */
        MAP_CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);

        /* Disabling high side voltage monitor/supervisor */
        MAP_PSS_disableHighSide();
        break;

    case HM_MODE:
        break;
        /* Set 2 flash wait states for Flash bank 0 and 1*/
        MAP_FlashCtl_setWaitState( FLASH_BANK0, 2);
        MAP_FlashCtl_setWaitState( FLASH_BANK1, 2);

        /* Initializes Clock System */
        //MAP_CS_setDCOFrequency(1000000);                //1-2MHz
        /* Disabling high side voltage monitor/supervisor */
        MAP_CS_setDCOCenteredFrequency( CS_DCO_FREQUENCY_24);
        MAP_CS_initClockSignal( CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);
        break;
    default:
        break;
    }
}

void ADC14_IRQHandler(void)
{
    updateBatteryLevel();
    void setOperationMode(void);
}

#endif /* SRC_TASKMANAGER_C_ */
