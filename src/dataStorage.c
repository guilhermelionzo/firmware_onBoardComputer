/*
 * dataStorage.c
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
 * \file dataStorage.c
 *
 * \brief dataStorage file
 *
 * \author Guilherme Lionço
 *
 */
#include "dataStorage.h"
#include "stdlib.h"

//------- SD VARIABLES --------------//
FATFS   sdDriver;           /*sd driver pointer*/
DIR     DI;
FILINFO FI;                 /**/
FIL     file;               /*file pointer*/
FIL     fileBin;
char    buffer[SIZE_OF_IMU_DATA];        /*buffer*/
UINT    br, bw;             /* File read/write count */
FRESULT fResult;            /*message handle*/
FILINFO FI;


//------- LOCAL VARIABLES --------------//

char buffer_timer[10]="";
char CTime;

extern QueueHandle_t xQueueIMU;
extern SemaphoreHandle_t semaphoreIMU;

ImuData imuData;
LocalTime localTime;

void getLocalTime(void);

void *dataStorage(void *pvParameters){


    //initializing the SPI protocol and mounting the SD card
    while(!initSD());

    memset(localTime.hour, 0x00, sizeof(localTime.hour));
    memset(localTime.minutes, 0x00, sizeof(localTime.hour));
    memset(localTime.seconds, 0x00, sizeof(localTime.hour));

    TickType_t tick;

    //try to open the telemetry file
    fResult = f_open(&file, "T_DATA.TXT",FA_OPEN_APPEND | FA_WRITE | FA_READ);

    //TODO: error handling

    while(1){

        //DEBUG SESSION
        #if DEBUG_SESSION
        GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN1); // PIN GREEN
        #endif

        //TODO: control of SPI interface

        //TODO: queue control

        //TIME STAMP
        getLocalTime();
        fResult=f_write(&file, localTime.hour, sizeof(localTime.hour), NULL);
        fResult=f_write(&file, ":", sizeof(":"), NULL);
        fResult=f_write(&file, localTime.minutes, sizeof(localTime.minutes), NULL);
        fResult=f_write(&file, ":", sizeof(":"), NULL);
        fResult=f_write(&file, localTime.seconds, sizeof(localTime.seconds), NULL);
        //xTaskGetTickCount();
        //MSP STATUS

        tick=xTaskGetTickCount();

        //SUBSYSTEM STATUS

        //DATA
        if(xSemaphoreTake(semaphoreIMU,2000)){

            xQueueReceive(xQueueIMU,&imuData,200);

            xSemaphoreGive(semaphoreIMU);
        }
        fResult=f_write(&file, " IMU:", sizeof(" IMU:"), NULL);
        fResult = f_write(&file, imuData.ax, sizeof(imuData.az), &bw);
        fResult = f_write(&file, imuData.ay, sizeof(imuData.az), &bw);
        fResult = f_write(&file, imuData.az, sizeof(imuData.az), &bw);
        fResult = f_write(&file, imuData.gx, sizeof(imuData.az), &bw);
        fResult = f_write(&file, imuData.gy, sizeof(imuData.az), &bw);
        fResult = f_write(&file, imuData.gz, sizeof(imuData.az), &bw);
        /* fResult = f_write(&file, imuData.mx, sizeof(imuData.az), &bw);
        fResult = f_write(&file, imuData.my, sizeof(imuData.az), &bw);
        fResult = f_write(&file, imuData.mz, sizeof(imuData.az), &bw);
         */

        //PAYLOAD

        fResult = f_write(&file,"\n", sizeof("\n"), &bw);
        f_sync(&file);

        //DEBUG SESSION
        #if DEBUG_SESSION
        GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN1);
        #endif

        setWatchDogBit_DATASTORAGE();
        // IF THE SYSTEM HAS LOW BATTERY THE TASK WILL USE A LONGER DELAY, OTHERWISE IT WILL USE A SHOETTER PERIOD
        (flag_lowBattery) ? vTaskDelay(DATA_STORAGE_TICK_PERIOD_LOW_BATTERY): vTaskDelay(DATA_STORAGE_TICK_PERIOD);


    }

    //vTaskDelete( NULL );

}

dataPacket readPacket(void){

    dataPacket data ={0};

    data = obcData;

    return data;
}

/*get the current time on MSP432*/
void getLocalTime(void){

    itoa(RTC_C_getCalendarTime().hours, &localTime.hour, 10);
    itoa(RTC_C_getCalendarTime().minutes, &localTime.minutes, 10);
    itoa(RTC_C_getCalendarTime().seconds, &localTime.seconds, 10);

}


/*initialize the SD CARD*/

int initSD(){

    CS_Init();              //init someting that I have to know, TODO

    SPI_Init(EUSCI_B0_BASE, SPI0MasterConfig);

    GPIO_Init(GPIO_PORT_P4, GPIO_PIN6);

    Interrupt_enableMaster();

    //mount the SD card
    fResult=f_mount(&sdDriver,"0",1);      //mount on the area 0 of the sd card

    if(fResult!=FR_OK){return FAILURE;}

    return SUCCESS;
}

void setWatchDogBit_DATASTORAGE(void){

    xEventGroupSetBits(WATCHDOG_EVENT_GROUP, DATASTORAGE_TASK_ID);

}
