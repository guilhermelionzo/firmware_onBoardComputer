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

/***** TASKS *****/
extern void *aodcsTask(void *pvParameters);
extern void *cameraTask(void *pvParameters);
extern void *dataStorage(void *pvParameters);
extern void *houseKeeping(void *pvParameters);
extern void *pptTask(void *pvParameters);
extern void *ttcTask(void *pvParameters);
extern void *watchDogTask(void *pvParameters);

/***** HANDLES *****/
QueueHandle_t xQueueIMU=NULL;
SemaphoreHandle_t semaphoreIMU=NULL;

void queueDataCrete();
void watchDogEventGroupCreate(void);


void taskCreate(){

    /*create queue to exchange data between the tasks*/
    queueDataCrete();

    watchDogEventGroupCreate();

    xTaskCreate(aodcsTask   , "AODCS Task"   , 1024, NULL, 1, NULL);
    xTaskCreate(cameraTask  , "CAMERA Task"  , 1024, NULL, 1, NULL);
    xTaskCreate(houseKeeping, "House Keeping", 1024, NULL, 1, NULL);
    xTaskCreate(dataStorage , "Data Storage" , 1024, NULL, 1, NULL);
    xTaskCreate(pptTask     , "PPT Task"     , 1024, NULL, 1, NULL);
    xTaskCreate(ttcTask     , "TT&C Task"    , 1024, NULL, 1, NULL);
    xTaskCreate(watchDogTask, "WTD Task"     , 1024, NULL, 5, NULL);

    //xTaskCreate(sensorTask, "Sensor", 1024, NULL, 1, NULL);


}

void watchDogEventGroupCreate(void){

    //create the event group for wtd
    WATCHDOG_EVENT_GROUP = xEventGroupCreate();
}

/* CREATE THE QUEUES TO SUPPORT THE DATA EXCHANGE BETWEEN THE TASKS*/
void queueDataCrete(){

    //creating a Queue to handle the data between the tasks
    xQueueIMU = xQueueCreate( 9,SIZE_OF_IMU_DATA*8);
    //xQueueIMU = xQueueCreate( 1,sizeof(obcData));

    semaphoreIMU = xSemaphoreCreateMutex();

}

void queueCommunicationCreate(){


}

#endif /* SRC_TASKMANAGER_C_ */
