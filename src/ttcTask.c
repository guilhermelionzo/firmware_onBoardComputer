/*
 * ttcTask.c
 *
 *  Created on: 11 de jun de 2019
 *      Author: Guilherme
 */


#include "ttcTask.h"


void setWatchDogBit_TTC(void);

void *ttcTask(void *pvParameters){


    /* The xLastWakeTime variable needs to be initialized with the current tick
     count. Note that this is the only time the variable is written to explicitly.
     After this xLastWakeTime is updated automatically internally within
     vTaskDelayUntil(). */
    portTickType xLastWakeTimeTTC = xTaskGetTickCount();


    while (1)
    {


        //DEBUG SESSION
        #if DEBUG_SESSION
        GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN2); // LED BLUE
        #endif

        uint8_t ttcComand;
        ttcComand = GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4);

        if(ttcComand==GPIO_INPUT_PIN_LOW){
            //don't set the bit in watchdog bits
        }else{
            setWatchDogBit_TTC();
        }

        vTaskDelay(10);

        //DEBUG SESSION
        #if DEBUG_SESSION
        GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN2); // LED BLUE
        #endif


        //TODO: the TT&C task
       (flag_lowBattery) ?
                vTaskDelayUntil(&xLastWakeTimeTTC,TTC_TASK_TICK_PERIOD_LOW_BATTERY) :
                vTaskDelayUntil(&xLastWakeTimeTTC,TTC_TASK_TICK_PERIOD);            //
    }
}

void setWatchDogBit_TTC(void){

    xEventGroupSetBits(WATCHDOG_EVENT_GROUP, TTC_TASK_ID);

}
