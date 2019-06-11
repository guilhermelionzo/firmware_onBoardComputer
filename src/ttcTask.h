/*
 * ttcTask.h
 *
 *  Created on: 11 de jun de 2019
 *      Author: Guilherme
 */

#ifndef SRC_TTCTASK_H_
#define SRC_TTCTASK_H_

//------- SYSTEM --------------//
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <src/systemDef.h>


#define TTC_TASK_TICK_PERIOD 100       //1 second
#define TTC_TASK_TICK_PERIOD_LOW_BATTERY TTC_TASK_TICK_PERIOD*3


void *ttcTask(void *pvParameters);


#endif /* SRC_TTCTASK_H_ */
