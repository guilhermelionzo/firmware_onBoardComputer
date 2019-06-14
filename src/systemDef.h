/*
 * systemDef.h
 *
 *  Created on: 17 de mar de 2019
 *      Author: Guilherme
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

#define AODCS_TASK_TICK_PERIOD 500/portTICK_RATE_MS       //=500/1ms=>period 500ms
#define AODCS_TASK_TICK_PERIOD_LOW_BATTERY AODCS_TASK_TICK_PERIOD*3 //1.5s

#define CAMERA_TASK_TICK_PERIOD 500/portTICK_RATE_MS       //500ms
#define CAMERA_TASK_TICK_PERIOD_LOW_BATTERY CAMERA_TASK_TICK_PERIOD*3 //1.5s

#define DATA_STORAGE_TICK_PERIOD 500/portTICK_RATE_MS       //500ms
#define DATA_STORAGE_TICK_PERIOD_LOW_BATTERY DATA_STORAGE_TICK_PERIOD*3 //1.5s

#define HOUSE_KEEPING_TICK_PERIOD 500/portTICK_RATE_MS       //500ms
#define HOUSE_KEEPING_TICK_PERIOD_LOW_BATTERY HOUSE_KEEPING_TICK_PERIOD*3 //1.5s

#define PPT_TASK_TICK_PERIOD 500/portTICK_RATE_MS       //500ms
#define PPT_TASK_TICK_PERIOD_LOW_BATTERY PPT_TASK_TICK_PERIOD*3 //1.5s

#define TTC_TASK_TICK_PERIOD 500/portTICK_RATE_MS       //500ms
#define TTC_TASK_TICK_PERIOD_LOW_BATTERY TTC_TASK_TICK_PERIOD*3 //1.5s

#define WATCHDOG_TASK_TICK_PERIOD 570/portTICK_RATE_MS       //570ms
#define WATCHDOG_TASK_TICK_PERIOD_LOW_BATTERY WATCHDOG_TASK_TICK_PERIOD*3 //2.1s

#define DEBUG_SESSION 1

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
    uint8_t payload2[100];
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

volatile int flag_lowBattery;

#define SUCCESS 1
#define FAILURE 0

#endif /* SYSTEMDEF_H_ */
