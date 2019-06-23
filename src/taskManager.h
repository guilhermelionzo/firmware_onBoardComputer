/*
 * taskManager.h
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
 * \file taskManager.h
 *
 * \brief taskManager file
 *
 * \author Guilherme Lionço
 *
 */

#ifndef SRC_TASKMANAGER_H_
#define SRC_TASKMANAGER_H_

//------- SYSTEM --------------//
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <src/systemDef.h>

#ifndef portMAX_24_BIT_NUMBER
#define portMAX_24_BIT_NUMBER ( 0xffffffUL )
#endif

void *taskManager(void *pvParameters);

#endif /* SRC_TASKMANAGER_H_ */

