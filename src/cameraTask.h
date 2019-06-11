/*
 * cameratask.h
 *
 *  Created on: 11 de jun de 2019
 *      Author: Guilherme
 */

#ifndef SRC_CAMERATASK_H_
#define SRC_CAMERATASK_H_


//------- SYSTEM --------------//
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <src/systemDef.h>


#define CAMERA_TASK_TICK_PERIOD 100       //1 second
#define CAMERA_TASK_TICK_PERIOD_LOW_BATTERY CAMERA_TASK_TICK_PERIOD*3


void *cameraTask(void *pvParameters);



#endif /* SRC_CAMERATASK_H_ */
