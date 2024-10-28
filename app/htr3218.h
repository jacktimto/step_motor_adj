#ifndef HTR3218_H
#define HTR3218_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "driver/i2c_master.h"
#include "my_i2c_master.h"

void i2c_master_init(void);

#define I2C_MASTER_FREQ_HZ 100000   /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS -1   //wair forever

#define blue 0x0000FF
#define pink 0xFF00FF
#define yellow 0xFFFF00
#define orange 0xFF8000
#define red 0xFF0000
#define green 0x00FF00

#define white_value 0x19
#define blue_value 0x10
#define red_value 0x10

// i2c init
void htr3218_init(void);
void Htr3218_reset(void);
void turn_on_RGB(uint16_t PM_value , uint8_t mybrightness);
void htr3218_all_led_on(void);

#endif