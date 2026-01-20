# PlatformIO Project for Waveshare ESP32-S3 Knob Touch


![Device image](https://www.waveshare.com/w/upload/thumb/6/67/900px-ESP32-S3-Knob-Touch-LCD-1.8-44.jpg/600px-900px-ESP32-S3-Knob-Touch-LCD-1.8-44.jpg)

## Links

[Waveshare device page](https://www.waveshare.com/product/arduino/boards-kits/esp32-s3/esp32-s3-knob-touch-lcd-1.8.htm)

[Waveshare wiki page](https://www.waveshare.com/product/arduino/boards-kits/esp32-s3/esp32-s3-knob-touch-lcd-1.8.htm)

## Background

Waveshare released a board with a description of how to support the board in Arduino, but didn't document how to develop in PlatformIO

PlatformIO's ongoing fall-out with Espressif means that esp32-platform hasn't been updated in the PlatformIO repos for a long time, resulting in the libraries being very outdated and don't have the APIs needed for the Waveshare device. The pioarduino project is building newer versions of esp32-platform for PlatformIO and this repo configures PlatformIO to use the pioarduino build. I've used this to build my own projects.

The LVGL demo was also a little... hacky. It uses the display driver from ESP-IDF, adds in a custom CS816 touch panel driver that uses the legacy i2c ESP-IDF driver (which is not compatible with the newer ESP-IDF i2c driver, and therefore also Arduino's Wire stack). The DRV2605 driver from SensorLib example isn't compatible with the LVGL demo (because it uses Arduino's Wire stack) and the chip on my board returns a TI ChipID that doesn't exist, so the haptics don't work at all.

## This repo
This repo uses the original LCD driver, but pulls in the CSTXXX and DRV2506 drivers from SensorLib, removing the old cst816 driver from the demo. It also provides access to the rotary knob via the simple bidi_switch_knob driver from Waveshare's demo.

The current SensorLib DRV2506 driver doesn't support the specific chip on the Waveshare board (or at least my one) so this repo currently pulls in a forked version of SensorLib with a patch to support this chip.

Finally, the LCD files are refactored from the example code, pulling the backlight control (lcd_bl_pwm_bsp) and touch drivers into lcd_bsp.c to clean up main.c and provide better readability.

## LVGL 8.4
This currently supports LVGL 8.4 (the version included in the Waveshare demo) but I will work on porting it to LVGL9 later. Instead of pulling in the library, I have used PlatformIO's libdeps to pull it in.

There's no lvgl.conf file in this project, instead this project uses build flags to configure LVGL. You'll find configuration for LVGL in platform.ini

## Structure
The libraries to support the encder, LCD and touch panel are in `lib` to keep the `src` directory for your project.