# PlatformIO Project for Waveshare ESP32-S3 Knob Touch


![Device image](https://www.waveshare.com/w/upload/thumb/6/67/900px-ESP32-S3-Knob-Touch-LCD-1.8-44.jpg/600px-900px-ESP32-S3-Knob-Touch-LCD-1.8-44.jpg)

## Links

[Waveshare device page](https://www.waveshare.com/product/arduino/boards-kits/esp32-s3/esp32-s3-knob-touch-lcd-1.8.htm)

[Waveshare wiki page](https://www.waveshare.com/product/arduino/boards-kits/esp32-s3/esp32-s3-knob-touch-lcd-1.8.htm)

## Background

Waveshare released a board with a description of how to support the board in Arduino, but didn't document how to develop in PlatformIO

PlatformIO's ongoing fall-out with Espressif means that esp32-platform hasn't been updated in the PlatformIO repos for a long time, resulting in the libraries being very outdated and don't have the APIs needed for the Waveshare device. The pioarduino project is building newer versions of esp32-platform for PlatformIO and this repo configures PlatformIO to use the pioarduino build. I've used this to build my own projects.

## LVGL 8.4
This currently supports LVGL 8.4 (the version included in the Waveshare demo) but I will work on porting it to LVGL9 later. Instead of pulling in the library, I have used PlatformIO's libdeps to pull it in.

There's no lvgl.conf file in this project, instead this project uses build flags to configure LVGL. You'll find configuration for LVGL in platform.ini

## Structure
The provided files from Waveshare are largely unedited, except for adding a screen timeout feature and removing the call to run the LVGL demo, which is now in main.c. The source files to support the LCD and touch panel are in `lib` to keep the `src` directory for your project.