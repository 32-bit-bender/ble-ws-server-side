/**
 * @file dht11_driver.c
 * @brief Implementation of the DHT11 temperature and humidity sensor driver
 *
 * This source file contains functions to initialize the DHT11 sensor,
 * read temperature and humidity data, and return the results.
 * 
 * Author: 32-bit-bender
 * Date: 18/11/2024
 */

#include "drv_dht11.h"
#include "esp_rom_sys.h"
#include "driver/gpio.h"
#include <stdio.h>

#define DHT11_TIMOUT                            10000
#define DHT11_PIN_HIGH                          1
#define DHT11_PIN_LOW                           0
#define DHT11_LOW_START_SIGNAL_DELAY_MS         18
#define DHT11_HIGH_START_SIGNAL_DELAY_US        30
#define DHT11_1ST_RESPONSE_DELAY_US             160 
#define DHT11_HIGH_SAMPLE_DELAY_US              30

static void dht11_delay_us(uint32_t us);
static void dht11_delay_ms(uint32_t ms);
static uint8_t dht11_read_byte (gpio_num_t pin_number);

/**
 * @brief Delays execution for a specified number of microseconds 
 * 
 * @param us Number of microseconds to delay 
 */
static void dht11_delay_us(uint32_t us)
{
    esp_rom_delay_us(us);
}

/**
 * @brief Delays execution for a specified number of milliseconds
 * 
 * @param ms Number of milliseconds to delay 
 */
static void dht11_delay_ms(uint32_t ms)
{
    uint32_t delay_us = 1000* ms;
    esp_rom_delay_us(delay_us);
}

/**
 * @brief Reads a byte of data from the DHT11 sensor
 * 
 * @param pin_number GPIO pin number connected to the DHT11 sensor
 * @return The byte read from the sensor, or 0xFF on timeout
 */
static uint8_t dht11_read_byte (gpio_num_t pin_number)
{
    uint8_t raw_byte = 0;
    uint8_t sample_count = 0;
    uint64_t timout_count = 0;
    int pin_state;
    for (sample_count=0;sample_count<8;sample_count++)
    {
        while((gpio_get_level(pin_number) == DHT11_PIN_LOW) &&
              (timout_count<DHT11_TIMOUT)) {
            timout_count++;
        }
        if(timout_count>=DHT11_TIMOUT) {
            return 0xFF;
        }
        timout_count = 0;
        
        dht11_delay_us(DHT11_HIGH_SAMPLE_DELAY_US);
        pin_state = gpio_get_level(pin_number);
        raw_byte |= (uint8_t) pin_state << (7 - sample_count);

        while((gpio_get_level(pin_number) == DHT11_PIN_HIGH) &&
              (timout_count<DHT11_TIMOUT)) {
            timout_count++;
        }
        if(timout_count>=DHT11_TIMOUT) {
            return 0xFF;
        }
        timout_count = 0;

    }
    return raw_byte;
}

/**
 * @brief Reads raw temperature and humidity data from the DHT11 sensor
 * 
 * @param pin_number GPIO pin number connected to the DHT11 sensor
 * @param raw_data Pointer to a structure to store the raw data
 * @return DHT11_SUCCESS on success, or DHT11_FAIL on checksum error
 */
dht11_err dht11_read_raw_data(gpio_num_t pin_number, dht11_raw_data* raw_data)
{
    uint32_t timout_count = 0;
    /* Init the GPIO PIN as pin*/
    //gpio_reset_pin(pin_number);
    gpio_set_direction(pin_number, GPIO_MODE_OUTPUT);
    
    /* ends a start low signal for at least 18ms.*/
    gpio_set_level(pin_number, DHT11_PIN_LOW);
    dht11_delay_ms(DHT11_LOW_START_SIGNAL_DELAY_MS);

    /*Pull line high for 20-30 to indicate readiness to read*/
    gpio_set_level(pin_number, DHT11_PIN_HIGH);
    dht11_delay_us(DHT11_HIGH_START_SIGNAL_DELAY_US);

    /*Change direction to start reading*/
    gpio_set_direction(pin_number, GPIO_MODE_INPUT);
    while((gpio_get_level(pin_number) == DHT11_PIN_HIGH) &&
          (timout_count<DHT11_TIMOUT))
    {
        timout_count++;
    }

    timout_count = 0;
    dht11_delay_us(DHT11_1ST_RESPONSE_DELAY_US);

    raw_data->hum_high = dht11_read_byte(pin_number);
    raw_data->hum_low  = dht11_read_byte(pin_number);
    raw_data->temp_high = dht11_read_byte(pin_number);
    raw_data->temp_low = dht11_read_byte(pin_number);
    raw_data->crc = dht11_read_byte(pin_number);
    
    uint32_t data_sum = raw_data->hum_high + raw_data->hum_low + raw_data->temp_high +
                        raw_data->temp_low;
    uint8_t calculated_checksum = (uint8_t) (data_sum & 0xFF);

    if(calculated_checksum != raw_data->crc) {
        return DHT11_FAIL;
    } else {
        return DHT11_SUCCESS;
    }
}