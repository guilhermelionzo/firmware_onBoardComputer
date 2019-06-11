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

#define DATA_STORAGE_TICK_PERIOD 100       //1 second

uint_fast8_t time;
char buffer_timer[10]="";
char CTime;

extern QueueHandle_t xQueueIMU;
extern SemaphoreHandle_t semaphoreIMU;

ImuData imuData;

void *dataStorage(void *pvParameters){


    //initializing the SPI protocol and mounting the SD card
    while(!initSD());

    RTC_C_startClock();

    //try to open the telemetry file
    fResult = f_open(&file, "T_DATA.TXT",FA_OPEN_APPEND | FA_WRITE | FA_READ);

    //TODO: error handling

    while(1){

        //TODO: control of SPI interface

        //TODO: queue control
        time = RTC_C_getCalendarTime().seconds;
        time=RTC_C_getCalendarTime().dayOfmonth;
        itoa(time,&buffer_timer,10);
        fResult=f_write(&file, buffer_timer, sizeof(buffer), NULL);

        fResult=f_write(&file, "IMU:", sizeof("IMU:"), NULL);

       if(xSemaphoreTake(semaphoreIMU,2000)){

            xQueueReceive(xQueueIMU,&imuData,200);

            xSemaphoreGive(semaphoreIMU);
       }
        /*double str = 123.14;

        fResult=f_write(&file, "Time:", sizeof("Time:"), NULL);

        fResult = f_write(&file,strCnv, sizeof(strCnv), &bw);

        fResult=f_write(&file, "; Value:", sizeof("; Value:"), NULL);
        char strT[10]="";
        dtoa(&strT, str);
        fResult = f_write(&file,strT, sizeof(strT), &bw);
*/

        fResult = f_write(&file, imuData.ax, sizeof(imuData.az), &bw);
        fResult = f_write(&file, imuData.ay, sizeof(imuData.az), &bw);
        fResult = f_write(&file, imuData.az, sizeof(imuData.az), &bw);
        fResult = f_write(&file, imuData.gx, sizeof(imuData.az), &bw);
        fResult = f_write(&file, imuData.gy, sizeof(imuData.az), &bw);
        fResult = f_write(&file, imuData.gz, sizeof(imuData.az), &bw);
        fResult = f_write(&file, imuData.mx, sizeof(imuData.az), &bw);
        fResult = f_write(&file, imuData.my, sizeof(imuData.az), &bw);
        fResult = f_write(&file, imuData.mz, sizeof(imuData.az), &bw);

        fResult = f_write(&file,";", sizeof(";"), &bw);
        f_sync(&file);

        vTaskDelay(DATA_STORAGE_TICK_PERIOD);

    }

    vTaskDelete( NULL );

}

dataPacket readPacket(void){

    dataPacket data ={0};

    data = obcData;

    return data;
}

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
