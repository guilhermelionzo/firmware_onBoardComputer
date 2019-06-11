/*
 * taskManager.c
 *
 *  Created on: 28 de mai de 2019
 *      Author: Guilherme
 */

#ifndef SRC_TASKMANAGER_C_
#define SRC_TASKMANAGER_C_

#include "taskManager.h"

extern void *houseKeeping(void *pvParameters);
extern void *dataStorage(void *pvParameters);
//extern void *sensorTask(void *pvParameters);

QueueHandle_t xQueueIMU=NULL;
SemaphoreHandle_t semaphoreIMU=NULL;

void queueCreate();

void taskCreate(){

    queueCreate();

    xTaskCreate(houseKeeping, "House Keeping", 1024, NULL, 1, NULL);

    xTaskCreate(dataStorage, "Data Storage", 1024, NULL, 1, NULL);

    //xTaskCreate(sensorTask, "Sensor", 1024, NULL, 1, NULL);


}

void queueCreate(){

    //creating a Queue to handle the data between the tasks
    xQueueIMU = xQueueCreate( 9,SIZE_OF_IMU_DATA*8);
    //xQueueIMU = xQueueCreate( 1,sizeof(obcData));

    semaphoreIMU = xSemaphoreCreateMutex();

}

#endif /* SRC_TASKMANAGER_C_ */
