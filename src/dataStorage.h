/*
 * dataStorage.h
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
 * along with FloripaSat-OBDH.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

 /**
 * \file dataStorage.h
 *
 * \brief dataStorage file
 *
 * \author Guilherme Lionço
 *
 */
#ifndef SRC_DATASTORAGE_H_
#define SRC_DATASTORAGE_H_

//------- RTOS --------------//
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

//------- SD --------------//
#include <util/Hardware/SPI_Driver.h>
#include <util/Hardware/GPIO_Driver.h>
#include <util/Hardware/CS_Driver.h>
#include <util/fatfs/ff.h>
#include <util/fatfs/diskio.h>
#include <util/Devices/MSPIO.h>

//------- SYSTEM --------------//
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define SUCCESS 1
#define FAILURE 0

//------- PROTOCOL CONFIGURATION --------------//
eUSCI_SPI_MasterConfig SPI0MasterConfig =
{
     EUSCI_B_SPI_CLOCKSOURCE_SMCLK,
     3000000,
     500000,
     EUSCI_B_SPI_MSB_FIRST,
     EUSCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT,
     EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH,
     EUSCI_B_SPI_3PIN
};

int initSD();

void *dataStorage(void *pvParameters);




#endif /* SRC_DATASTORAGE_H_ */
