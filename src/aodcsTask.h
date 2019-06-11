/*
 * aodcsTask.h
 *
 *  Created on: 11 de jun de 2019
 *      Author: Guilherme
 */

#ifndef SRC_AODCSTASK_H_
#define SRC_AODCSTASK_H_


//------- SYSTEM --------------//
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <src/systemDef.h>


#define AODCS_TASK_TICK_PERIOD 300       //1 second
#define AODCS_TASK_TICK_PERIOD_LOW_BATTERY AODCS_TASK_TICK_PERIOD*3


void *aodcsTask(void *pvParameters);


#endif /* SRC_AODCSTASK_H_ */
