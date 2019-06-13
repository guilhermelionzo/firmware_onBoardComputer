/*
 * taskManager.c
 *
 *  Created on: 28 de mai de 2019
 *      Author: Guilherme
 */

#ifndef SRC_TASKMANAGER_C_
#define SRC_TASKMANAGER_C_

#include "taskManager.h"

//tasks
extern void *aodcsTask(void *pvParameters);
extern void *cameraTask(void *pvParameters);
extern void *dataStorage(void *pvParameters);
extern void *houseKeeping(void *pvParameters);
extern void *pptTask(void *pvParameters);
extern void *ttcTask(void *pvParameters);
extern void *watchDogTask(void *pvParameters);

//extern void *sensorTask(void *pvParameters);

QueueHandle_t xQueueIMU=NULL;
SemaphoreHandle_t semaphoreIMU=NULL;

void queueCreate();
void watchDogEventGroupCreate(void);


void taskCreate(){

    queueCreate();
    watchDogEventGroupCreate();

    xTaskCreate(aodcsTask   , "AODCS Task"   , 1024, NULL, 1, NULL);
    xTaskCreate(cameraTask  , "CAMERA Task"  , 1024, NULL, 2, NULL);
    xTaskCreate(houseKeeping, "House Keeping", 1024, NULL, 3, NULL);
    xTaskCreate(dataStorage , "Data Storage" , 1024, NULL, 3, NULL);
    xTaskCreate(pptTask     , "PPT Task"     , 1024, NULL, 1, NULL);
    xTaskCreate(ttcTask     , "TT&C Task"    , 1024, NULL, 2, NULL);
    xTaskCreate(watchDogTask, "WTD Task"     , 1024, NULL, 5, NULL);

    //xTaskCreate(sensorTask, "Sensor", 1024, NULL, 1, NULL);


}

void watchDogEventGroupCreate(void){

    //create the event group fot wtd
    WATCHDOG_EVENT_GROUP = xEventGroupCreate();
}

void queueCreate(){

    //creating a Queue to handle the data between the tasks
    xQueueIMU = xQueueCreate( 9,SIZE_OF_IMU_DATA*8);
    //xQueueIMU = xQueueCreate( 1,sizeof(obcData));

    semaphoreIMU = xSemaphoreCreateMutex();

}

#endif /* SRC_TASKMANAGER_C_ */
