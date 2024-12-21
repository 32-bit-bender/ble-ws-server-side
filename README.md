# Weather Station Introduction

This is one of a two-part project for a weather station using two `ESP32`s, the `DHT11` sensor, and the `ILI9341` TFT display. The main goal of this project is to read and advertise the sensor data using the ESP32 on the server side and display the data on the display side. Both ESP32s use the NimBLE stack, with the display side acting as the central and the server side as the peripheral.

I created this project mainly to learn about BLE and become comfortable using the NimBLE stack on the ESP-IDF framework.

Links to each part of the Weather Station:

* Server side: https://github.com/32-bit-bender/ble-ws-server-side
* Display side: https://github.com/32-bit-bender/ble-ws-display-side

### Server Side

The server side uses a very simple BLE profile:
* The device uses a custom MAC address: `f2:64:74:61:65:77`
* To advertise data, the GATT profile has one service containing one read characteristic
* The sensor data is updated in the attribute through a periodic FreeRTOS task

### Hardware Connection

      ESP Board                            DHT11 Sensor
┌──────────────────────┐              ┌────────────────────┐
│             GND      ├──────────────┤ Ground             │
│                      │              │                    │
│             5V       ├──────────────┤ VCC                │
│                      │              │                    │
│             GPIO18   |◄────────────►│ Signal             │
│                      │              │                    │
└──────────────────────┘              └────────────────────┘

### Build, Flash, and Monitor

* Prerequisites: You must have ESP-IDF installed on your machine. If that's not the case, follow the guide below:
https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/index.html

* Clone the project
* Build using `idf.py build`
* Flash using `idf.py flash -p <PORT>` with PORT being the USB port connected to your ESP board (for example, `idf.py flash -p /dev/ttyUSB0`)
* To see the ESP32's serial monitor, you can run `idf.py monitor -p <PORT>` (To exit the serial monitor, type ``Ctrl-]``)

* To interact with the BLE profile, you can use `chrome://bluetooth-internals/#adapter` on your Chrome browser or applications like LightBlue or nRF Connect on your smartphone