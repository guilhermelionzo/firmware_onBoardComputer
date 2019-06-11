/*
 * pptTask.h
 *
 *  Created on: 11 de jun de 2019
 *      Author: Guilherme
 */

#ifndef SRC_PPTTASK_H_
#define SRC_PPTTASK_H_

//------- SYSTEM --------------//
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <src/systemDef.h>


#define PPT_TASK_TICK_PERIOD 300       //1 second
#define PPT_TASK_TICK_PERIOD_LOW_BATTERY PPT_TASK_TICK_PERIOD*3


void *pptTask(void *pvParameters);


#endif /* SRC_PPTTASK_H_ */
