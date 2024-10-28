#define NDEBUG
#include "assert.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "driver/rmt_tx.h"
#include "driver/gpio.h"
#include "driver/gptimer.h"
#include "esp_log.h"
#include "esp_rom_sys.h"
#include "74HC595.h"
#include "step_motor.h"
#include "my_adc.h"
#include "my_uart.h"
#include "motor.h"
#include "eeprom.h"
#include "htr3236.h"
#include "htr3218.h"
#include "ir_nec_transceiver.h"
#include "freertos/semphr.h"
#include "SGP40_SHT40.h"
#include "my_gptimer.h"

uint16_t target_motor_speed = 1680;
uint8_t eeprom_buffer[16] = {0};
uint8_t TX_buffer[3] = {0, 0x03, 0xE8};
#define NDEBUG

void system_init(void)
{   
    gptimer_init();
    gpio_74HC595_init();
    uart_init();
    i2c_master_init();
    htr3236_init();
    htr3218_init();
    SGP40_SHT40_init();
    gpio_set_level(PTC_EN, 0); // default turn off ptc
    turn_on_RGB(1, 0xFF);
    htr3218_all_led_on();
}

step_motor_config_t step1 = {
    .speed = 300,
    .angle = 1,
    .step_motor_mask = 0x0001,

};

void app_main()
{   
    /*二维数组*/
    uint8_t two_dimension_array[2][3] = {{1,2,3},{4,5,6}};

    /*遍历数组*/
    for(int i = 0;i < 3;i++)
    {
        for (int j = 0; j < 3; j++)    //越界访问
        {
            ESP_LOGI("二维数组","array[%d][%d]=%d,addr%p",i,j,two_dimension_array[i][j],&two_dimension_array[i][j]);
        }
        
    }




    assert(0);
    system_init();
    vTaskDelay(100 / portTICK_PERIOD_MS);

    vTaskDelay(pdMS_TO_TICKS(10));
    Htr3236_write(step1.speed, 0x10);
    // turn on all led

    xTaskCreate(step_motor_control, "top step motor", 8192, &step1, tskIDLE_PRIORITY + 1, NULL);
    // xTaskCreate(vadc_read, "adc_period_read", 4096, NULL, 1, NULL);
    // xTaskCreate(motor_set, "motor_regulate", 4096, NULL, 1, NULL);
    // xTaskCreate(ir_nec_transceiver, "ir_nec_transceiver", 4096, NULL, 1, NULL);
    // xTaskCreate(eeprom_period_write,"eeprom_period_write",4096,NULL,1,NULL);
    // xTaskCreate(SGP40_SHT40_read, "SHT40 SGP40 read", 8192, NULL, 1, NULL);
    // xTaskCreate(pm1003_read,"pm1003_read",8192,NULL,1,NULL);
    xTaskCreate(esp32_write_to_byd, "esp32_write_to_byd", 8192, NULL, 1, NULL);

    while (1)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}