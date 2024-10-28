#ifndef _74HC595_H_
#define _74HC595_H_

#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "esp_rom_sys.h"

// 74hc595 driver
#define SER 5
#define RCK 4
#define SCK 2
#define PWM_EN 32
#define PTC_EN 21
#define PM25_EN 27
#define BYD_EN 12

void Init_74HC595(void);
void gpio_74HC595_init(void);
void Send_74HC595_16bit(uint16_t value);
void Send_74HC595_8bit(uint8_t value);
#endif