/*
 * systemDef.h
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
 * \file systemDef.h
 *
 * \brief systemDef file
 *
 * \author Guilherme Lionço
 *
 */

#ifndef SYSTEMDEF_H_
#define SYSTEMDEF_H_

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#include "util/stringUtil.h"

#define SIZE_OF_IMU_DATA 7
#define LOW_BATTERY_STATUS 1

#define AODCS_TASK_TICK_PERIOD 1000/portTICK_RATE_MS       //=1000/1ms => period 1000ms
#define AODCS_TASK_TICK_PERIOD_LOW_BATTERY AODCS_TASK_TICK_PERIOD*3 //3s

#define CAMERA_TASK_TICK_PERIOD 1000/portTICK_RATE_MS       //1000ms
#define CAMERA_TASK_TICK_PERIOD_LOW_BATTERY CAMERA_TASK_TICK_PERIOD*3 //3s

#define DATA_STORAGE_TICK_PERIOD 1000/portTICK_RATE_MS       //1000ms
#define DATA_STORAGE_TICK_PERIOD_LOW_BATTERY DATA_STORAGE_TICK_PERIOD*3 //3s

#define HOUSE_KEEPING_TICK_PERIOD 1000/portTICK_RATE_MS       //1000ms( pdMS_TO_TICKS( 300UL ) )//
#define HOUSE_KEEPING_TICK_PERIOD_LOW_BATTERY HOUSE_KEEPING_TICK_PERIOD*3 //3s

#define PPT_TASK_TICK_PERIOD 1000/portTICK_RATE_MS       //1000ms
#define PPT_TASK_TICK_PERIOD_LOW_BATTERY PPT_TASK_TICK_PERIOD*3 //3s

#define TTC_TASK_TICK_PERIOD 1000/portTICK_RATE_MS       //1000ms( pdMS_TO_TICKS( 300UL ) )
#define TTC_TASK_TICK_PERIOD_LOW_BATTERY TTC_TASK_TICK_PERIOD*3 //3s

#define WATCHDOG_TASK_TICK_PERIOD 1000/portTICK_RATE_MS       //1000ms
#define WATCHDOG_TASK_TICK_PERIOD_LOW_BATTERY WATCHDOG_TASK_TICK_PERIOD*3 //3s

#define DEBUG_SESSION 1

typedef struct
{

    uint8_t varCounter;
    uint8_t varReset;

} DataFlash;

typedef struct
{
    char ax[SIZE_OF_IMU_DATA];
    char ay[SIZE_OF_IMU_DATA];
    char az[SIZE_OF_IMU_DATA];
    char gx[SIZE_OF_IMU_DATA];
    char gy[SIZE_OF_IMU_DATA];
    char gz[SIZE_OF_IMU_DATA];
    char mx[SIZE_OF_IMU_DATA];
    char my[SIZE_OF_IMU_DATA];
    char mz[SIZE_OF_IMU_DATA];
} ImuData;

typedef struct
{
    char seconds[2];
    char minutes[2];
    char hour[2];
} LocalTime;

typedef struct
{
    uint16_t package_flags;
    //obdh
    uint8_t system_status[6];
    ImuData imuData;
    uint8_t obc_sensors[5];
    uint8_t internalTemperature;
    uint8_t systick[4];
    uint8_t solar_panels[12];
    uint8_t transceiver[85];
    //eps
    uint8_t adc_solar_panels[18];
    uint8_t msp432_adc[8];
    uint8_t battery_monitor[21];
    uint8_t ads1248[21];
    uint8_t task_scheduler[1];
    //payloads
    uint8_t payload1[7];
    uint8_t payload2[7];
} dataPacket;

/* calendar struct. */
static RTC_C_Calendar calendarConfig;

/* The queue used by both tasks. */
static QueueHandle_t xQueueMPU = NULL;

/*Event Handler*/
//Watchdog
EventGroupHandle_t WATCHDOG_EVENT_GROUP;

//task ID
#define AODCS_TASK_ID         (1<<0)
#define CAMERA_TASK_ID        (1<<1)
#define DATASTORAGE_TASK_ID   (1<<2)
#define HOUSEKEEPING_TASK_ID  (1<<3)
#define PPT_TASK_ID           (1<<4)
#define TTC_TASK_ID           (1<<5)
#define ALL_TASK_IDS  (AODCS_TASK_ID|CAMERA_TASK_ID|DATASTORAGE_TASK_ID|HOUSEKEEPING_TASK_ID|PPT_TASK_ID|TTC_TASK_ID)

volatile dataPacket obcData;
volatile DataFlash dataFlash;

volatile int flag_lowBattery;

#define SUCCESS 1
#define FAILURE 0

#endif /* SYSTEMDEF_H_ */
