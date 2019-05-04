/*
 * systemDef.h
 *
 *  Created on: 17 de mar de 2019
 *      Author: Guilherme
 */

#ifndef SYSTEMDEF_H_
#define SYSTEMDEF_H_

typedef struct {
    uint16_t package_flags;
    //obdh
    uint8_t system_status      [6];
    uint8_t imu                [24];
    uint8_t obc_sensors        [6];
    uint8_t systick            [4];
    uint8_t solar_panels       [12];
    uint8_t transceiver        [85];
    //eps
    uint8_t adc_solar_panels   [18];
    uint8_t msp432_adc         [8];
    uint8_t battery_monitor    [21];
    uint8_t ads1248            [21];
    uint8_t task_scheduler     [1];
    //payloads
    uint8_t payload1           [7];
    uint8_t payload2           [100];
} data_packet_t;




#endif /* SYSTEMDEF_H_ */
