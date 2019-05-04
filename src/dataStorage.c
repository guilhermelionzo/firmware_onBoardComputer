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

//------- VARIABLES --------------//
FATFS   sdDriver;           /*sd driver pointer*/
DIR     DI;
FILINFO FI;                 /**/
FIL     file;               /*file pointer*/
char    buffer[100];        /*buffer*/
UINT    br, bw;             /* File read/write count */
FRESULT fResult;            /*message handle*/
FILINFO FI;

#define DATA_STORAGE_TICK_PERIOD 1000       //1 second

uint_fast8_t time;
char CTime;

void *dataStorage(void *pvParameters){


    //initializing the SPI protocol and mounting the SD card
    while(!initSD());

    RTC_C_startClock();

    //try to open the telemetry file
    fResult = f_open(&file, "t_data.txt","a");

    while(fResult!=FR_OK){//TODO: do the error handling

        fResult = f_open(&file, "FILE.TXT","a");

        f_close(&file);

        f_rename("FILE.TXT", "t_data.txt");

        fResult = f_open(&file, "t_data.txt","a");
    }

    while(1){

        //TODO: control of SPI interface

        //TODO: queue control
        time = RTC_C_getCalendarTime().seconds;

        //memcpy(CTime,(char*)&time, sizeof(time));

        fResult=f_write(&file, "teste:", sizeof("teste:"), NULL);

        fResult=f_write(&file, time, sizeof(time), &bw);

        fResult=f_write(&file, "|", sizeof("|"), &bw);

        //if(fResult!=FR_OK){while(1);}

        f_sync(&file);

        vTaskDelay(DATA_STORAGE_TICK_PERIOD);

    }



}

int initSD(){

    CS_Init();              //init someting that I have to know, TODO

    SPI_Init(EUSCI_B0_BASE, SPI0MasterConfig);

    GPIO_Init(GPIO_PORT_P5, GPIO_PIN0);

    Interrupt_enableMaster();

    //mount the SD card
    fResult=f_mount(&sdDriver,"0",1);      //mount on the area 0 of the sd card

    if(fResult!=FR_OK){return FAILURE;}

    return SUCCESS;
}
