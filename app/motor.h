#ifndef  MOTOR_H 
#define  MOTOR_H 

#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "driver/pulse_cnt.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_sleep.h"

#define LEDC_TIMER LEDC_TIMER_0
#define LEDC_MODE LEDC_LOW_SPEED_MODE  //LEDC_HIGH_SPEED_MODE 设置为低速模式
#define LEDC_OUTPUT_IO1 (33) // Define the output GPIO    PWM1
// #define LEDC_OUTPUT_IO2 (32) // Define the output GPIO    PWM2
// #define LEDC_OUTPUT_IO3 (33) // Define the output GPIO    PWM3
#define LEDC_CHANNEL LEDC_CHANNEL_1
#define LEDC_DUTY_RES LEDC_TIMER_12_BIT // Set duty resolution to 13 bits

//#define LEDC_DUTY (1815)                // Set duty 1000/4096


#define LEDC_FREQUENCY (7500)           // Frequency in Hertz. Set frequency at 7.5 kHz


void motor_init(int PWM_IO);
void motor_start(void);
void pwm_regulate(void);

void motor_set(void * parameter);

#endif