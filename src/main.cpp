#include <Arduino.h>

//display
#include "lcd_bsp.h"
#include "cst816.h"
#include "lcd_bl_pwm_bsp.h"
#include "lcd_config.h"
#include "lvgl.h"

//encoder
#include "bidi_switch_knob.h"
static const char *TAG = "encoder";

#define EXAMPLE_ENCODER_ECA_PIN    8
#define EXAMPLE_ENCODER_ECB_PIN    7

#define SET_BIT(reg,bit) (reg |= ((uint32_t)0x01<<bit))
#define CLEAR_BIT(reg,bit) (reg &= (~((uint32_t)0x01<<bit)))
#define READ_BIT(reg,bit) (((uint32_t)reg>>bit) & 0x01)
#define BIT_EVEN_ALL (0x00ffffff)

EventGroupHandle_t knob_even_ = NULL;

static knob_handle_t s_knob = 0;
static void _knob_left_cb(void *arg, void *data) {
  uint8_t eventBits_ = 0;
  SET_BIT(eventBits_,0);
  xEventGroupSetBits(knob_even_,eventBits_);
}
static void _knob_right_cb(void *arg, void *data) {
  uint8_t eventBits_ = 0;
  SET_BIT(eventBits_,1);
  xEventGroupSetBits(knob_even_,eventBits_);
}

//use this function to define what you want the knob to do
static void user_encoder_loop_task(void *arg) {
  int8_t vol = 10;
  for(;;) {
    EventBits_t even = xEventGroupWaitBits(knob_even_,BIT_EVEN_ALL,pdTRUE,pdFALSE,pdMS_TO_TICKS(5000));
    if(READ_BIT(even,0)) {
      // rotate counter-clockwise, e.g.
      //vol--;
      //if(vol<=0)
      //vol = 0;
    }
    if(READ_BIT(even,1)) {
      // rotate clockwise, e.g.
      //vol++;
      //if(vol>=100)
      //vol = 100;
    }
  }
}

//buzzer
//#include "SensorDRV2605.hpp"
//SensorDRV2605 drv;
//uint8_t effect = 1;

lv_obj_t * label;

static void event_handler(lv_event_t * e) {
  lv_event_code_t code = lv_event_get_code(e);

  if(code == LV_EVENT_CLICKED) {
    lv_label_set_text(label, "Clicked!");
  }
}

void setup() {
  Serial.begin(115200);

  //display initialization
  Touch_Init();
  lcd_lvgl_Init();
  lcd_bl_pwm_bsp_init(LCD_PWM_MODE_255);

  //encoder initialization
  knob_even_ = xEventGroupCreate();
  knob_config_t cfg = {
    .gpio_encoder_a = EXAMPLE_ENCODER_ECA_PIN,
    .gpio_encoder_b = EXAMPLE_ENCODER_ECB_PIN,
  };
  s_knob = iot_knob_create(&cfg);
  iot_knob_register_cb(s_knob, KNOB_LEFT, _knob_left_cb, NULL);
  iot_knob_register_cb(s_knob, KNOB_RIGHT, _knob_right_cb, NULL);
  xTaskCreate(user_encoder_loop_task, "user_encoder_loop_task", 3000, NULL, 2, NULL);

  //optional, sets a screen timeout. The screen will turn off after this many seconds. Touching the screen turns it back on.
  setDisplayTimeout(10);

  lv_obj_t * screen = lv_obj_create(NULL);
  label = lv_label_create(screen);
  lv_label_set_text(label, "Hello PlatfomIO!");
  lv_obj_center(label);

  lv_obj_t * btn1 = lv_btn_create(screen);
  lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, NULL);
  lv_obj_align(btn1, LV_ALIGN_CENTER, 0, -40);

  lv_obj_t * btnlabel = lv_label_create(btn1);
  lv_label_set_text(btnlabel, "Button");
  lv_obj_center(btnlabel);
  
  lv_scr_load(screen);

  

  //buzzer initialization
  //if (!drv.begin(Wire, SENSOR_SDA, SENSOR_SCL)) {
  //  Serial.println("Failed to find DRV2605 - check your wiring!");
  //  while (1) {
  //    delay(1000);
  //  }
  //}
  //Serial.println("Init DRV2605 Sensor success!");

  //drv.setWaveform(0, effect);
  //drv.setWaveform(1, 0);
  //drv.run();
}

void loop() {

//you can use this to integrate display brightness, perhaps add it into bsp.c as a function to set display brightness
#ifdef Backlight_Testing
  setUpdutySubdivide(LCD_PWM_MODE_255);
  delay(1000);
  setUpdutySubdivide(LCD_PWM_MODE_200);
  delay(1000);
  setUpdutySubdivide(LCD_PWM_MODE_150);
  delay(1000);
  setUpdutySubdivide(LCD_PWM_MODE_100);
  delay(1000);
  setUpdutySubdivide(LCD_PWM_MODE_50);
  delay(1000);
  setUpdutySubdivide(LCD_PWM_MODE_0);
  delay(1000);
#endif
}