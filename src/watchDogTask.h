/*
 * watchDogTask.h
 *
 *  Created on: 13 de jun de 2019
 *      Author: Guilherme
 */

#ifndef SRC_WATCHDOGTASK_H_
#define SRC_WATCHDOGTASK_H_


//------- SYSTEM --------------//
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <src/systemDef.h>


void *watchDogTask(void *pvParameters);



#endif /* SRC_WATCHDOGTASK_H_ */
