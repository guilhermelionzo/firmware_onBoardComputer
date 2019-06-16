/*
 * sensorTask.h
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
 * \file sensorTask.h
 *
 * \brief sensorTask file
 *
 * \author Guilherme Lionço
 *
 */
#ifndef SRC_SENSORTASK_H_
#define SRC_SENSORTASK_H_

//------- RTOS --------------//
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

//------- SYSTEM --------------//
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <src/systemDef.h>



#define SENSOR_TASK_TICK_PERIOD 100

void *sensorTask(void *pvParameters);


#endif /* SRC_SENSORTASK_H_ */
