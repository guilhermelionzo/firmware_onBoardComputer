/*
 * houseKeeping.h
 *
 *  Created on: 4 de mai de 2019
 *      Author: Guilherme
 */

#ifndef SRC_HOUSEKEEPING_H_
#define SRC_HOUSEKEEPING_H_

//------- RTOS --------------//
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

//------- SYSTEM --------------//
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <src/systemDef.h>

//------- SENSOR --------------//
#include <interface/MPU9250.h>
#include "src/sensorTask.h"

#endif /* SRC_HOUSEKEEPING_H_ */
