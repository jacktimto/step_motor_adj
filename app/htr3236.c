#include "htr3236.h"

#define HTR3236_ADDR 0x3F

// 使能 htr3236 输出
uint8_t write_buf2[2] = {0x4A, 0x00};
uint8_t write_buf3[2] = {0x25, 0x11};

extern i2c_master_bus_handle_t bus_handle;
i2c_master_dev_handle_t htr3236_handle;

i2c_device_config_t htr3236_cfg =
    {
        .dev_addr_length = I2C_ADDR_BIT_7,
        .device_address = HTR3236_ADDR,
        .scl_speed_hz = 100000,
};

void htr3236_init(void)
{
  ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &htr3236_cfg, &htr3236_handle));
  Htr3236_reset();
}

void Htr3236_write(uint16_t num, uint8_t my_brightness)
{
  uint8_t write_buf0[2] = {0x00, 0x01};
  ESP_ERROR_CHECK(i2c_master_transmit(htr3236_handle, write_buf0, sizeof(write_buf0), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS));

  // 选择通道
  uint8_t write_buf1[] = {0x26, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
                          0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
                          0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
                          0x01, 0x01, 0x01, 0x01, 0x01}; // constant write data in
  i2c_master_transmit(htr3236_handle, write_buf1, sizeof(write_buf1), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);

  Display_Num(num, my_brightness);

  i2c_master_transmit(htr3236_handle, write_buf2, sizeof(write_buf2), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);

  i2c_master_transmit(htr3236_handle, write_buf3, sizeof(write_buf3), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
}

/**
 * led对应htr3236的outx引脚
 *
 *
 *            PM1.0(22)                       PM2.5(23)               PM10 (24)      TVOC(25)
 *
 *
 *
 *            led(15)                         led(8)                     led(1)
 *                                                                                                   wifi(26)
 *
 *    led(20)         led(16)         led(13)         led(9)     led(6)            led(2)           hins(27)
 *             led(21)                         led(14)                    led(7)
 *
 *    led(19)         led(17)         led(12)         led(10)     led(5)          led(3)
 *            led(18)                         led(11)                    led(4)                     filter(28)
 *
 *      RGB-red(29)                  RGB-green(30) RGB-blue(31)
 *
 * 程序上可以分区显示
 *可以通过输入的数字,来选择哪几个灯亮
 *
 */
void Display_Num(uint16_t num, uint8_t brightness)
{
  uint8_t number[3] = {0, 0, 0};
  uint8_t write_buf1[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint8_t write_buf2[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint8_t write_buf3[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint8_t write_buf4[2] = {0x05,brightness};
  uint8_t write_buf5[2] = {0x05,0x00};
  if (num > 999)
  {
    if(num > 1999)
    num = 1999;
    num = num -1000;
    i2c_master_transmit(htr3236_handle, write_buf4, sizeof(write_buf4), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
  }
  else
  {
    i2c_master_transmit(htr3236_handle, write_buf5, sizeof(write_buf5), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
  }
  number[0] = num % 10; // number[2]   number[1]  number[0]
  number[1] = (num / 10) % 10;
  number[2] = num / 100;
  switch (number[0])
  {
  case 0:
    uint8_t write_buf1_0[8] = {0x17,brightness, brightness, brightness, brightness, brightness, brightness, 0x00};
    memcpy(write_buf1, write_buf1_0, 8);
    break;
  case 1:
    uint8_t write_buf1_1[8] = {0x17, 0x00, 0x00, brightness, brightness, 0x00, 0x00, 0x00};
    memcpy(write_buf1, write_buf1_1, 8);
    break;
  case 2:
    uint8_t write_buf1_2[8] = {0x17, 0x00, brightness, brightness, 0x00, brightness, brightness, brightness};
    memcpy(write_buf1, write_buf1_2, 8);
    break;
  case 3:
    uint8_t write_buf1_3[8] = {0x17, 0x00, brightness, brightness, brightness, brightness, 0x00, brightness};
    memcpy(write_buf1, write_buf1_3, 8);
    break;
  case 4:
    uint8_t write_buf1_4[8] = {0x17, brightness, 0x00, brightness, brightness, 0x00, 0x00, brightness};
    memcpy(write_buf1, write_buf1_4, 8);
    break;
  case 5:
    uint8_t write_buf1_5[8] = {0x17, brightness, brightness, 0x00, brightness, brightness, 0x00, brightness};
    memcpy(write_buf1, write_buf1_5, 8);
    break;
  case 6:
    uint8_t write_buf1_6[8] = {0x17, brightness, brightness, 0x00, brightness, brightness, brightness, brightness};
    memcpy(write_buf1, write_buf1_6, 8);
    break;
  case 7:
    uint8_t write_buf1_7[8] = {0x17, 0x00, brightness, brightness, brightness, 0x00, 0x00, 0x00};
    memcpy(write_buf1, write_buf1_7, 8);
    break;
  case 8:
    uint8_t write_buf1_8[8] = {0x17, brightness, brightness, brightness, brightness, brightness, brightness, brightness};
    memcpy(write_buf1, write_buf1_8, 8);
    break;
  case 9:
    uint8_t write_buf1_9[8] = {0x17, brightness, brightness, brightness, brightness, brightness, 0x00, brightness};
    memcpy(write_buf1, write_buf1_9, 8);
    break;
  }
  i2c_master_transmit(htr3236_handle, write_buf1, sizeof(write_buf1), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);

  switch (number[1])
  {
  case 0:
    uint8_t write_buf2_0[8] = {0x10,brightness, brightness, brightness, brightness, brightness, brightness, 0x00};
    memcpy(write_buf2, write_buf2_0, 8);
    break;
  case 1:
    uint8_t write_buf2_1[8] = {0x10, 0x00, 0x00, brightness, brightness, 0x00, 0x00, 0x00};
    memcpy(write_buf2, write_buf2_1, 8);
    break;
  case 2:
    uint8_t write_buf2_2[8] = {0x10, 0x00, brightness, brightness, 0x00, brightness, brightness, brightness};
    memcpy(write_buf2, write_buf2_2, 8);
    break;
  case 3:
    uint8_t write_buf2_3[8] = {0x10, 0x00, brightness, brightness, brightness, brightness, 0x00, brightness};
    memcpy(write_buf2, write_buf2_3, 8);
    break;
  case 4:
    uint8_t write_buf2_4[8] = {0x10, brightness, 0x00, brightness, brightness, 0x00, 0x00, brightness};
    memcpy(write_buf2, write_buf2_4, 8);
    break;
  case 5:
    uint8_t write_buf2_5[8] = {0x10, brightness, brightness, 0x00, brightness, brightness, 0x00, brightness};
    memcpy(write_buf2, write_buf2_5, 8);
    break;
  case 6:
    uint8_t write_buf2_6[8] = {0x10, brightness, brightness, 0x00, brightness, brightness, brightness, brightness};
    memcpy(write_buf2, write_buf2_6, 8);
    break;
  case 7:
    uint8_t write_buf2_7[8] = {0x10, 0x00, brightness, brightness, brightness, 0x00, 0x00, 0x00};
    memcpy(write_buf2, write_buf2_7, 8);
    break;
  case 8:
    uint8_t write_buf2_8[8] = {0x10, brightness, brightness, brightness, brightness, brightness, brightness, brightness};
    memcpy(write_buf2, write_buf2_8, 8);
    break;
  case 9:
    uint8_t write_buf2_9[8] = {0x10, brightness, brightness, brightness, brightness, brightness, 0x00, brightness};
    memcpy(write_buf2, write_buf2_9, 8);
    break;
  }

  i2c_master_transmit(htr3236_handle, write_buf2, sizeof(write_buf2), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);

  switch (number[2])
  {
    case 0:
    uint8_t write_buf3_0[8] = {0x09,brightness, brightness, brightness, brightness, brightness, brightness, 0x00};
    memcpy(write_buf3, write_buf3_0, 8);
    break;
  case 1:
    uint8_t write_buf3_1[8] = {0x09, 0x00, 0x00, brightness, brightness, 0x00, 0x00, 0x00};
    memcpy(write_buf3, write_buf3_1, 8);
    break;
  case 2:
    uint8_t write_buf3_2[8] = {0x09, 0x00, brightness, brightness, 0x00, brightness, brightness, brightness};
    memcpy(write_buf3, write_buf3_2, 8);
    break;
  case 3:
    uint8_t write_buf3_3[8] = {0x09, 0x00, brightness, brightness, brightness, brightness, 0x00, brightness};
    memcpy(write_buf3, write_buf3_3, 8);
    break;
  case 4:
    uint8_t write_buf3_4[8] = {0x09, brightness, 0x00, brightness, brightness, 0x00, 0x00, brightness};
    memcpy(write_buf3, write_buf3_4, 8);
    break;
  case 5:
    uint8_t write_buf3_5[8] = {0x09, brightness, brightness, 0x00, brightness, brightness, 0x00, brightness};
    memcpy(write_buf3, write_buf3_5, 8);
    break;
  case 6:
    uint8_t write_buf3_6[8] = {0x09, brightness, brightness, 0x00, brightness, brightness, brightness, brightness};
    memcpy(write_buf3, write_buf3_6, 8);
    break;
  case 7:
    uint8_t write_buf3_7[8] = {0x09, 0x00, brightness, brightness, brightness, 0x00, 0x00, 0x00};
    memcpy(write_buf3, write_buf3_7, 8);
    break;
  case 8:
    uint8_t write_buf3_8[8] = {0x09, brightness, brightness, brightness, brightness, brightness, brightness, brightness};
    memcpy(write_buf3, write_buf3_8, 8);
    break;
  case 9:
    uint8_t write_buf3_9[8] = {0x09, brightness, brightness, brightness, brightness, brightness, 0x00, brightness};
    memcpy(write_buf3, write_buf3_9, 8);
    break;
  }
  i2c_master_transmit(htr3236_handle, write_buf3, sizeof(write_buf3), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
}

void Htr3236_reset(void)
{
  uint8_t write_buf5[2] = {0x4F, 0x00}; // htr3236复位命令
  i2c_master_transmit(htr3236_handle, write_buf5, sizeof(write_buf5), 100 / portTICK_PERIOD_MS);
}

// if want change brigtness ,insert variable my_brightness
void turn_on_RGB_htr3236(uint16_t PM_value, uint8_t mybrightness)
{
  uint32_t color;
  // 29-Red  30-Green  31-Blue
  uint8_t RGB_config[4] = {0x1D, 0x00, 0x00, 0x00};
  if (PM_value < 11)
  {
    color = blue;
    RGB_config[1] = ((color & 0xFF0000) >> 16) / mybrightness;
    RGB_config[2] = ((color & 0x00FF00) >> 8) / mybrightness;
    RGB_config[3] = (color & 0x0000FF) / mybrightness;
  }
  if ((PM_value > 10) && (PM_value < 26))
  {
    color = green;
    RGB_config[1] = ((color & 0xFF0000) >> 16) / mybrightness;
    RGB_config[2] = ((color & 0x00FF00) >> 8) / mybrightness;
    RGB_config[3] = (color & 0x0000FF) / mybrightness;
  }
  if ((PM_value > 25) && (PM_value < 36))
  {
    color = yellow;
    RGB_config[1] = ((color & 0xFF0000) >> 16) / mybrightness;
    RGB_config[2] = ((color & 0x00FF00) >> 8) / mybrightness;
    RGB_config[3] = (color & 0x0000FF) / mybrightness;
  }
  if ((PM_value > 35) && (PM_value < 51))
  {
    color = orange;
    RGB_config[1] = ((color & 0xFF0000) >> 16) / mybrightness;
    RGB_config[2] = ((color & 0x00FF00) >> 8) / mybrightness;
    RGB_config[3] = (color & 0x0000FF) / mybrightness;
  }
  if (PM_value > 50)
  {
    color = red;
    RGB_config[1] = ((color & 0xFF0000) >> 16) / mybrightness;
    RGB_config[2] = ((color & 0x00FF00) >> 8) / mybrightness;
    RGB_config[3] = (color & 0x0000FF) / mybrightness;
  }
  i2c_master_transmit(htr3236_handle, RGB_config, sizeof(RGB_config), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);

  i2c_master_transmit(htr3236_handle, write_buf2, sizeof(write_buf2), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);

  i2c_master_transmit(htr3236_handle, write_buf3, sizeof(write_buf3), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
}