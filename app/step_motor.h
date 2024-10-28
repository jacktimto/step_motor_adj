#ifndef STEP_MOTOR_H
#define STEP_MOTOT_H

#include "freertos/FreeRTOS.h"
// #include "esp_log.h"
// #include "driver/gpio.h"
// #include "esp_rom_sys.h"

// 声明一个step_motor的结构体
typedef struct
{
    uint16_t step_motor_mask;  //0000: no step motor selected ,0001: step motor1 selected ,0010: step motor2 selected ,100: step motor3 selected 
                              //step1:顶部电机  step2:前方的电机   step3:摇头电机
    int16_t angle;
    uint16_t speed;
}step_motor_config_t;

// parameter1 代表顶部的步进电机CN8,parameter2 代表前方的步进电机CN1, parameter3 代表摇头的步进电机CN2
// 涉及参数 angle(拍数), speed
void step_motor_control(void * prvparameter);

#endif