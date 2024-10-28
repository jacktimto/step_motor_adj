#include "my_gptimer.h"
#include "step_motor.h"

#include "driver/gpio.h"
uint8_t flag_1ms = 0;
extern step_motor_config_t step1;

static bool gptimer_on_alarm_callback(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx)
{
    
    return 0;
}
void gptimer_init(void)
{
    gptimer_handle_t gptimer = NULL;
    gptimer_config_t gptimer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = 1000000, // 1MHz ,1tick = 1us
    };
    ESP_ERROR_CHECK(gptimer_new_timer(&gptimer_config, &gptimer));

    // set gptimer alarm action
    gptimer_alarm_config_t alarm_config = {
        .reload_count = 0,
        .alarm_count = 1000, // 每隔1ms触发重载
        .flags.auto_reload_on_alarm = true,
    };
    ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer, &alarm_config));

    // 设置回调函数
    gptimer_event_callbacks_t gptimer_cb = {
        .on_alarm = gptimer_on_alarm_callback,
    };
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &gptimer_cb, NULL)); // 注册回调函数
    ESP_ERROR_CHECK(gptimer_enable(gptimer));
    ESP_ERROR_CHECK(gptimer_start(gptimer));
}

void gptimer_delay(uint16_t time)
{

}