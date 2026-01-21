# PlatformIO Project for Waveshare ESP32-S3 1.8" Knob Touch


![Device image](https://www.waveshare.com/w/upload/thumb/6/67/900px-ESP32-S3-Knob-Touch-LCD-1.8-44.jpg/600px-900px-ESP32-S3-Knob-Touch-LCD-1.8-44.jpg)

## Links

[Waveshare device page](https://www.waveshare.com/product/arduino/boards-kits/esp32-s3/esp32-s3-knob-touch-lcd-1.8.htm)

[Waveshare wiki page](https://www.waveshare.com/product/arduino/boards-kits/esp32-s3/esp32-s3-knob-touch-lcd-1.8.htm)

## Contents
Preconfigured PlatformIO workspace that includes:
* Configured LVGL8 environment
* LCD, touch panel and haptic drivers and sample code. Touch panel and haptic drivers are from a forked copy of SensorLib to support the haptic chip
* USB-CDC (for Serial output)
* Knob / Dial library from Waveshare, but it's a simple two-way switch on the side of the device so you can roll your own easily
* LVGL configuration through `platformio.ini`

## How to use
Clone this repo and open the folder in PlatformIO. Configuration changes for LVGL (default fonts, included fonts, etc.) can be configured in platformio.ini using build flags. For example, to add support for Montserrat 18, add the following line to `platformio.ini` under build_flags:

```
build_flags=
...
  -DLV_FONT_MONTSERRAT_18
```

## Background

The quality and feel of the Waveshare 1.8" Dial are very good - the case is machined aluminum, the dial moves nicely and has a nice feel and tehs screen is bright with good color accuracy. But getting the device working is hard:

#### LCD
The display is driven by the less well-known SH8601 chip that's not well supported. The only driver for the display is part of the ESP-IDF framework, which will work with Arduino although the code is native ESP-IDF code. That doesn't matter too much when you're using LVGL since it's all hidden away in `lcd_bsp.c`. I made some minor modifications to move the code to C++ to add better support for the touch panel.

#### Touch panel
The touch panel uses the CS816 chip. It's also not very widely supported, although better than the display. The driver supplied by Waveshare in the demo is also for the ESP-IDF framework but uses the legacy ESP-IDF i2c driver stack that isn't compatible with the Arduino framework, meaning you can't access any other device on the i2c bus via Arduino if you're using the Waveshare-supplied touch driver.

Since the SensorLib library supports both the touch panel and the haptic chip, I dropped the ESP-IDF driver from Waveshare in favor of the SensorLib library.

#### Haptic
Haptics (buzz) are provided by the SRV2605 chip. The demo uses SensorLib, although my particular device didn't work. I discovered that the chip is reporting a ChipID that's not in the TI documentation. I've created a fork of SensorLib with support for the undocumented ChipID that works with the device. That fork is configured in `platform.ini` under libdeps.

#### Audio and encoder
Some parts aren't so hard though: the encoder (dial) is simply a two-way switch, so that's easy and the DAC is a standard PCM5100 series chip that's supported by many libraries. TF cards are well supported by Arduino, so you just need to configure the pins and open the SPI bus.

I left the dial library from Waveshare in place and use is documented in the supplied demo code.

#### esp32-platform
PlatformIO's ongoing fall-out with Espressif means that esp32-platform hasn't been updated in the PlatformIO repos for a _long_ time, resulting in the libraries being very outdated. The latest version in the PlatformIO repo doesn't support the APIs needed for the newer SH8601 display driver, so the build will fail. However, the pioarduino project is building newer versions of esp32-platform for PlatformIO and this repo configures PlatformIO to use the pioarduino build.

## Structure
The libraries to support the encder, LCD and touch panel are in `lib` to keep the `src` directory for your project.

## What's next
- Support for LVGL9