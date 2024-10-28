#include "motor.h"
#include "pcnt.h"

extern uint16_t target_motor_speed;
uint16_t *p = &target_motor_speed;
uint16_t LEDC_DUTY = 1800; // 300rpm- 540  500rpm-780   800rpm-1170   1000rpm-1400 1330rpm-1800
int pulse_count = 0;       // motor FG count
uint32_t speed_delta = 0;
extern pcnt_unit_handle_t my_pcnt_unit;

void motor_init(int PWM_IO)
{
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_MODE,
        .duty_resolution = LEDC_DUTY_RES,
        .timer_num = LEDC_TIMER,
        .freq_hz = LEDC_FREQUENCY,
        .clk_cfg = LEDC_AUTO_CLK};
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL,
        .timer_sel = LEDC_TIMER,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = PWM_IO,
        .duty = 0, // Set duty to 0%
        .hpoint = 0};
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

void motor_start(void)
{
    motor_init(LEDC_OUTPUT_IO1);
    // motor_init(LEDC_OUTPUT_IO2);
    // motor_init(LEDC_OUTPUT_IO3);
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_1, LEDC_DUTY));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_1));
}

void pwm_regulate(void)
{
    uint8_t motor_step = 0;
    if (speed_delta < 50)
    {
        motor_step = 5;
        if (speed_delta < 10)
        {
            motor_step = 0;
        }
    }
    else
    {
        motor_step = 15;
    }

    if (target_motor_speed > pulse_count * 4)
    {
        LEDC_DUTY = LEDC_DUTY + motor_step;
    }
    else
    {
        LEDC_DUTY = LEDC_DUTY - motor_step;
    }
    if (LEDC_DUTY > 4000)
    {
        LEDC_DUTY = 4000;
    }
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_1, LEDC_DUTY));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_1));
}

void motor_set(void *parameter)
{
    pcnt_init();
    motor_start();
    while (1)
    {
        ESP_ERROR_CHECK(pcnt_unit_clear_count(my_pcnt_unit));
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        ESP_ERROR_CHECK(pcnt_unit_get_count(my_pcnt_unit, &pulse_count));
        speed_delta = abs(target_motor_speed - pulse_count * 4);
        ESP_LOGI("test", "current_speed:%d,current_duty:%d,target_speed:%d", pulse_count * 4, LEDC_DUTY, target_motor_speed);
        pwm_regulate();
    }
}