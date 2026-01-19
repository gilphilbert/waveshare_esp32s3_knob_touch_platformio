#include "cst816.h"
#include "esp_err.h"
#include "lcd_config.h"

#define TEST_I2C_PORT I2C_NUM_0
i2c_master_dev_handle_t dev_handle;

uint8_t I2C_writr_buff(uint8_t addr,uint8_t reg,uint8_t *buf,uint8_t len) {
  uint8_t ret;
  uint8_t *pbuf = (uint8_t*)malloc(len+1);
  pbuf[0] = reg;
  for(uint8_t i = 0; i<len; i++)
  {
    pbuf[i+1] = buf[i];
  }
  ret = i2c_master_transmit(dev_handle, pbuf, len+1, 1000);
  free(pbuf);
  pbuf = NULL;
  return ret;
}

uint8_t I2C_read_buff(uint8_t addr,uint8_t reg,uint8_t *buf,uint8_t len) {
  uint8_t ret;
  ret = i2c_master_transmit_receive(dev_handle, &reg, 1, buf, len, 1000);

  return ret;
}

uint8_t I2C_master_write_read_device(uint8_t addr,uint8_t *writeBuf,uint8_t writeLen,uint8_t *readBuf,uint8_t readLen) {
  uint8_t ret;
  ret = i2c_master_transmit_receive(dev_handle, writeBuf, writeLen, readBuf, readLen, 1000);
  return ret;
}

void Touch_Init(void) {
  i2c_master_bus_config_t i2c_bus_config = {};
  i2c_bus_config.clk_source = I2C_CLK_SRC_DEFAULT;
  i2c_bus_config.i2c_port = TEST_I2C_PORT;
  i2c_bus_config.scl_io_num = (gpio_num_t)SENSOR_SCL;
  i2c_bus_config.sda_io_num = (gpio_num_t)SENSOR_SDA;
  i2c_bus_config.flags.enable_internal_pullup = true;
  
  i2c_master_bus_handle_t bus_handle;
  ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_bus_config, &bus_handle));

  i2c_device_config_t dev_cfg = {
      .dev_addr_length = I2C_ADDR_BIT_LEN_7,
      .device_address = EXAMPLE_TOUCH_ADDR,
      .scl_speed_hz = 100000, //don't know what the correct port speed is!
  };

  ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_cfg, &dev_handle));

  uint8_t data = 0x00;
  I2C_writr_buff(EXAMPLE_TOUCH_ADDR,0x00,&data,1); //Switch to normal mode
}
uint8_t getTouch(uint16_t *x,uint16_t *y)
{
  uint8_t GetNum = 0;
  uint8_t data[7] = {0};
  I2C_read_buff(EXAMPLE_TOUCH_ADDR,0x00,data,7);
  GetNum = data[2];
  if(GetNum)
  {
    *x = ((uint16_t)(data[3] & 0x0f)<<8) + (uint16_t)data[4];
    *y = ((uint16_t)(data[5] & 0x0f)<<8) + (uint16_t)data[6];
    return 1;
  }
  return 0;
}


