# Smart Meter Zigbee

This project presents a Zigbee device for the IR port on the Norax 3D+ smart meter. The hardware used includes the Seeed XIAO nRF52840 microcontroller and a Volkszähler Hichi Smartmeter TTL. The microcontroller fits perfectly inside the 3D printed case of the Hichi TTL after making some adjustments to the case's opening with sandpaper.

Due to the size of the case, the USB-C port on the XIAO nRF52840 does not fit as-is. To resolve this, one may used sandpaper to carefully modify the opening on the case until it fit snugly around the USB-C port on the XIAO nRF52840. Be cautious while sanding to avoid damaging the case.

The Seeed board comes with an integrated bootloader, which does not support Zigbee. To enable Zigbee functionality, I used the Nordic nRF Connect SDK.

Follow the instructions below to set up the development environment, build the application, and flash the firmware onto the XIAO nRF52840.


## Prerequisites
- Install ``west`` and the Nordic SDK
- Clone this repository and initialize the git submodules

## Build and Flash
1. Install the Nordic nRF Connect SDK as described [here](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/getting_started/installing.html#gs-installing) on Linux. Use the main branch of the SDK instead of v2.3.0.
2. Build the application using the following command:
```sh
west build -b xiao_ble
```
3. Flash the firmware to the device using a Raspberry Pi connected to the SWD ports on the XIAO. Use the following pin setup:


|   Raspberry Pi   |  XIAO  |
|:----------------:|:------:|
|     any 3.3V     |   3V3  |
|      any GND     |   GND  |
| GPIO 11 (PIN 23) | SWDCLK |
| GPIO 25 (PIN 22) |  SWDIO |

4. Perform a flash mass erase before the first flash:
```sh
openocd -f interface/raspberrypi-native.cfg -c 'transport select swd' -f target/nrf52.cfg -c init -c 'reset init' -c halt -c 'nrf5 mass_erase' -c exit
```
5. Flash the firmware:
```sh
openocd -f interface/raspberrypi-native.cfg -c 'transport select swd' -f target/nrf52.cfg -c init -c 'reset init' -c halt -c 'program ./build/zephyr/merged.hex verify' -c reset -c exit
```
You may also combine steps 3 and 4 into a single command if desired:
```sh
openocd -f interface/raspberrypi-native.cfg -c 'transport select swd' -f target/nrf52.cfg -c init -c 'reset init' -c halt -c 'nrf5 mass_erase' -c 'program ./build/zephyr/zephyr.hex verify' -c reset -c exit
```

## Zigbee2mqtt Configuration
1. Add the following lines to your zigbee2mqtt configuration:
```yaml
external_converters:
  - smz.js
```
2. Copy the `misc/smz.js` file from this repository to your zigbee2mqtt directory.

## Assembling the Hardware
1. Connect the Seeed XIAO nRF52840 to the Volkszähler Hichi Smart Meter TTL using the following connections:


| Hichi | XIAO |
|:-----:|:----:|
|  VCC  |  3V3 |
|  GND  |  GND |
|   RX  |  RX  |
|   TX  |  TX  |
2. Connect the XIAO's USB port to a power source.

The microcontroller will wait for the first SML message and then start the Zigbee network steering. Enable the permit join on zigbee2mqtt.

## Zigbee Firmware Over-The-Air (FOTA)
1. Add the following lines to your zigbee2mqtt configuration:
```
ota:
  zigbee_ota_override_index_location: custom_ota_index.json
```

2. Create the `custom_ota_index.json` file inside your zigbee2mqtt directory and insert custom OTA files:
```json
[
    {
        "url": "127F-0141-01020004-SMZ.zigbee"
    }
]
```

3. Place the `127F-0141-01020004-SMZ.zigbee` file in the zigbee2mqtt directory, containing a new firmware version.
