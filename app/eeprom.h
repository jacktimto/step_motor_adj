#ifndef EEPROM
#define EEPROM

#include "freertos/FreeRTOS.h"
#include "driver/i2c_master.h"
#include "my_i2c_master.h"
#include "esp_err.h"
#include "esp_log.h"

uint16_t eeprom_read(uint16_t device_addr,uint8_t len);
void eeprom_write(uint16_t device_addr, uint8_t *pdata, uint8_t len);

#endif