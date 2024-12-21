/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include "esp_log.h"
#include "nvs_flash.h"
/* BLE */
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "host/util/util.h"
#include "console/console.h"
#include "services/gap/ble_svc_gap.h"
#include "bleprph.h"
/*DHT11*/
#include "drv/drv_dht11.h"

#define DHT11_PIN  18
#define CONFIG_SENSOR_READ_PERIOD 2000


void nvs_init (void);
void periodic_sensor_reads(void* pvParameters);


void
app_main(void)
{
    (void) nvs_init ();
    (void) nimble_server_init ();
    xTaskCreate(periodic_sensor_reads, "Periodic sensor reading task",
                2048, NULL, 1, NULL);
}


void nvs_init (){
    /* Initialize NVS — it is used to store PHY calibration data */
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

void periodic_sensor_reads(void* pvParameters)
{
    dht11_raw_data raw_data;
    while (1)
    {
        dht11_err read_status;
        read_status = dht11_read_raw_data(DHT11_PIN, &raw_data);

        if (read_status == DHT11_SUCCESS)
        {
            ble_characteristic_value[0] = raw_data.temp_high;
            ble_characteristic_value[1] = raw_data.temp_low;
            ble_characteristic_value[2] = raw_data.hum_high;
            ble_characteristic_value[3] = raw_data.hum_low;
            ble_characteristic_value[4] = raw_data.crc;
        }

        vTaskDelay(CONFIG_SENSOR_READ_PERIOD/portTICK_PERIOD_MS);
    }
}