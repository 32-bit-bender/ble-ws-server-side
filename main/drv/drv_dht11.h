/**
 * @file drv_dht11.h
 * @brief Driver for the DHT11 temperature and humidity sensor
 *
 * This driver provides functions to initialize the DHT11 sensor,
 * read temperature and humidity data, and return the results.
 * 
 * Usage:
 * - Initialize the sensor using DHT11_Init().
 * - Call DHT11_Read() to obtain temperature and humidity values.
 *
 * Author: 32-bit-bender
 * Date: 18/11/2024
 */
#ifndef DRV_DHT11_H
#define DRV_DHT11_H

#include <stdlib.h>
#include "driver/gpio.h"

typedef struct 
{
    uint8_t hum_high;
    uint8_t hum_low;
    uint8_t temp_high;
    uint8_t temp_low;
    uint8_t crc;
} dht11_raw_data;

typedef enum
{
    DHT11_SUCCESS = 0,
    DHT11_FAIL    = -1
} dht11_err;

dht11_err dht11_read_raw_data(gpio_num_t pin_number, dht11_raw_data* raw_data);

#endif //DRV_DHT11_H
