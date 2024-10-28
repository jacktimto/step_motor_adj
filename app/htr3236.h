#ifndef HTR3236_H
#define HTR3236_H

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
void i2c_master_init(void);
void htr3236_init(void);
void Htr3236_write(uint16_t num, uint8_t my_brightness); // !!!  delight all led,call this function to dispaly num
void Htr3236_reset(void);
void Display_Num(uint16_t num, uint8_t brightness); // display num input
void turn_on_RGB_htr3236(uint16_t PM_value , uint8_t mybrightness);

#endif