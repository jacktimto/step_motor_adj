#include "step_motor.h"
#include "74HC595.h"

uint32_t count = 0;

extern step_motor_config_t step1;

void step_motor_control(void *prvparameter)
{
    uint16_t step_motor_CCW[8] = {0x1, 0x3, 0x2, 0x6, 0x4, 0xC, 0x8, 0x9};

    // uint16_t step_motor_mask = pt_step_motor->step_motor_mask;
    uint16_t step1_select = 0x0001, step2_select = 0x0100, step3_select = 0x1000; // uint16_t step1_select = 0x0001, step2_select = 0x0100, step3_select = 0x1000;

    while (1)
    {
        int16_t angle = step1.angle;
        uint16_t speed = step1.speed;
        if ((angle & 0x8000) == 0) // 0 ,正转 ; 1 ,反转
        {
            for (int8_t i = 0; i < 8; i++)
            {
                Send_74HC595_16bit(step_motor_CCW[7 - i] * step1_select + step_motor_CCW[7 - i] * step2_select + step_motor_CCW[7 - i] * step3_select);
                esp_rom_delay_us(speed*10);
            }
        }
        else
        {
            for (int8_t i = 0; i < 8; i++)
            {
                Send_74HC595_16bit(step_motor_CCW[i] * step1_select + step_motor_CCW[i] * step2_select + step_motor_CCW[i] * step3_select);
                esp_rom_delay_us(speed*10);
            }
        }
    }
}
