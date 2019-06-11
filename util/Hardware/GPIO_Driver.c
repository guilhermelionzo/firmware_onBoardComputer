#include "GPIO_Driver.h"

void GPIO_Init(uint32_t Port, uint16_t Pins)
{
    MAP_GPIO_setAsOutputPin(Port, Pins);

    if(Port==GPIO_PORT_P4&& Pins==GPIO_PIN6){GPIO_Low(GPIO_PORT_P4, GPIO_PIN6);}
}

void GPIO_High(uint32_t Port, uint16_t Pins)
{
    MAP_GPIO_setOutputHighOnPin(Port, Pins);
}

void GPIO_Low(uint32_t Port, uint16_t Pins)
{
    MAP_GPIO_setOutputLowOnPin(Port, Pins);
}
