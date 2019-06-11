/*
 * sensorTask.c
 *
 *  Created on: 2 de jun de 2019
 *      Author: Guilherme
 */
#include "sensorTask.h"

volatile uint32_t cal30;
volatile uint32_t cal85;
volatile float calDifference;
volatile float tempC;
volatile float tempF;

//internal temperature sensor P5.5 (A0)

static uint16_t resultsBuffer[8];


void adcInit(void);
void getTemperature(int16_t *temperatureBuffer);

void *sensorTask(void *pvParameters){

    adcInit();

    /* Zero-filling buffer */
    memset(resultsBuffer, 0x00, 8 * sizeof(uint16_t));

    int16_t temperatureValue;

    while(1){

        getTemperature(&temperatureValue);

        ADC14_getMultiSequenceResult(resultsBuffer);

        vTaskDelay(SENSOR_TASK_TICK_PERIOD);
    }

    /* Going to sleep */
    /*while (1)
    {
        PCM_gotoLPM0();
    }*/

}

void getTemperature(int16_t *temperatureBuffer){

    int16_t adcValue;

    adcValue = ((ADC14_getResult(ADC_MEM0) - cal30) * 55);

    *temperatureBuffer = (adcValue / calDifference) + 30.0f;

}

void ADC14_IRQHandler(void)
{

    uint64_t status;
    int16_t conRes;

    status = ADC14_getEnabledInterruptStatus();
    ADC14_clearInterruptFlag(status);

    if(status & ADC_INT0)
    {
        conRes = ((ADC14_getResult(ADC_MEM0) - cal30) * 55);
        tempC = (conRes / calDifference) + 30.0f;
        tempF = tempC * 9.0f / 5.0f + 32.0f;
    }

}


void adcInit(void){

    /* Initializing ADC (MCLK/1/1) with temperature sensor routed */
    ADC14_enableModule();
    ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1,
                     ADC_TEMPSENSEMAP);

    /* Configuring GPIOs for Analog In */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5,
            GPIO_PIN3 | GPIO_PIN2 | GPIO_PIN1| GPIO_PIN0, GPIO_TERTIARY_MODULE_FUNCTION);

    /* Configuring ADC Memory (ADC_MEM0 - ADC_MEM7 (A0 - A5)  with repeat)
     * with internal 2.5v reference */
    ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM5, true);
    ADC14_configureConversionMemory(ADC_MEM0,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A0, false);
    ADC14_configureConversionMemory(ADC_MEM1,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A1, false);
    ADC14_configureConversionMemory(ADC_MEM2,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A2, false);
    ADC14_configureConversionMemory(ADC_MEM3,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A3, false);
    ADC14_configureConversionMemory(ADC_MEM4,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A4, false);
    ADC14_configureConversionMemory(ADC_MEM5,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A5, false);

    /* Configuring the sample/hold time for 192 */
    ADC14_setSampleHoldTime(ADC_PULSE_WIDTH_192,ADC_PULSE_WIDTH_192);

    ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);

    REF_A_enableTempSensor();
    REF_A_setReferenceVoltage(REF_A_VREF2_5V);
    REF_A_enableReferenceVoltage();

    //data to calibrate the temperature sensor
    cal30 = SysCtl_getTempCalibrationConstant(SYSCTL_2_5V_REF,
            SYSCTL_30_DEGREES_C);
    cal85 = SysCtl_getTempCalibrationConstant(SYSCTL_2_5V_REF,
            SYSCTL_85_DEGREES_C);
    calDifference = cal85 - cal30;

    /* Triggering the start of the sample */
    ADC14_enableConversion();
    ADC14_toggleConversionTrigger();
}
