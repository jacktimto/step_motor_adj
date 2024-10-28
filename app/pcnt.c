#include "pcnt.h"

#define PCNT_HIGH_LIMIT 1000 // 因为电机转速2200rpm  ,2200/4 =550
#define PCNT_LOW_LIMIT -100
#define CHAN_GPIO_2 34 // 设置FG中断的IO num 22
#define CHAN_GOIO_1 22 // 外接hall

pcnt_unit_config_t my_pcnt_unit_config = {
    .high_limit = PCNT_HIGH_LIMIT,
    .low_limit = PCNT_LOW_LIMIT,
};
pcnt_unit_handle_t my_pcnt_unit = NULL; // 设置unit

pcnt_chan_config_t my_pcnt_chan_config = {
    .edge_gpio_num = CHAN_GPIO_2,
    .level_gpio_num = -1, // unused
};
pcnt_channel_handle_t my_pcant_chan = NULL; // 设置channel

pcnt_glitch_filter_config_t my_glith_filter_config = {
    // 设置毛刺过滤
    .max_glitch_ns = 1000, // 把低于 1us宽度的毛刺过滤
};

static bool pcnt_on_reach(pcnt_unit_handle_t unit, const pcnt_watch_event_data_t *edata, void *user_ctx)
{
    BaseType_t high_task_wakeup;
    QueueHandle_t queue = (QueueHandle_t)user_ctx; // user_ctx 用户上下文
    // send watch point to queue, from this interrupt callback
    xQueueSendFromISR(queue, &(edata->watch_point_value), &high_task_wakeup);

    // return whether a high priority task has been waken up by this function
    return (high_task_wakeup == pdTRUE);
}

void pcnt_init(void)
{
    ESP_ERROR_CHECK(pcnt_new_unit(&my_pcnt_unit_config, &my_pcnt_unit));                                                            // 安装pcnt unit
    ESP_ERROR_CHECK(pcnt_new_channel(my_pcnt_unit, &my_pcnt_chan_config, &my_pcant_chan));                                          // 安装pcnt 通道
    ESP_ERROR_CHECK(pcnt_channel_set_edge_action(my_pcant_chan, PCNT_CHANNEL_EDGE_ACTION_INCREASE, PCNT_CHANNEL_EDGE_ACTION_HOLD)); // 上升沿计数
    ESP_ERROR_CHECK(pcnt_unit_set_glitch_filter(my_pcnt_unit, &my_glith_filter_config));                                            // 设置毛刺过滤

    ESP_ERROR_CHECK(pcnt_unit_add_watch_point(my_pcnt_unit, 100)); // 添加观察点 一般是过零点 最大值 最小值
    ESP_ERROR_CHECK(pcnt_unit_clear_count(my_pcnt_unit));          // 使观察点生效

    pcnt_event_callbacks_t cbs = {
        .on_reach = pcnt_on_reach,
    };
    QueueHandle_t queue = xQueueCreate(10, sizeof(int));
    ESP_ERROR_CHECK(pcnt_unit_register_event_callbacks(my_pcnt_unit, &cbs, queue));

    pcnt_unit_enable(my_pcnt_unit); // 使能 单元
    pcnt_unit_start(my_pcnt_unit);  // 启动 单元
}
