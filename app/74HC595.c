#include "74HC595.h"

#define GPIO_OUTPUT_PIN_SEL (1ULL << SER) | (1ULL << SCK) | (1ULL << RCK) | (1ULL << PWM_EN) | (1ULL << PTC_EN) | (1ULL << PM25_EN) \
                            | (1ULL <<BYD_EN)

void Init_74HC595(void)
{
    gpio_set_level(SER, 1);
    gpio_set_level(SCK, 1);
    gpio_set_level(RCK, 1);
    gpio_set_level(PWM_EN, 1);
    gpio_set_level(PM25_EN,1);
    gpio_set_level(BYD_EN,1);
}

void gpio_74HC595_init(void)
{
    gpio_config_t io_conf = {};
    // disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    // set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    // bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    // disable pull-down mode
    io_conf.pull_down_en = 0;
    // disable pull-up mode
    io_conf.pull_up_en = 1;
    // configure GPIO with the given settings
    gpio_config(&io_conf);

    Init_74HC595();
    Send_74HC595_16bit(0x0);
}


// FIFO   Q7 ~Q0
    void Send_74HC595_16bit(uint16_t value)
{
    uint8_t i = 0;
    for (i = 0; i < 16; i++)
    {
        if (value & 0x8000)
        {
            gpio_set_level(SER, 1);
        }
        else
        {
            gpio_set_level(SER, 0);
        }
        value = value << 1;

        gpio_set_level(SCK, 0);    //sck上升沿 存储值
        esp_rom_delay_us(1);
        gpio_set_level(SCK, 1);
    }

    gpio_set_level(RCK, 0);   //rck上升沿 输出值
    esp_rom_delay_us(1);
    gpio_set_level(RCK, 1);
}

// 单个 74HC595
void Send_74HC595_8bit(uint8_t value)
{
    uint8_t i = 0;
    for (i = 0; i < 8; i++)
    {
        gpio_set_level(SER, value >> 7);
        value = value << 1;

        gpio_set_level(SCK, 0);
        esp_rom_delay_us(1); // 这里的延时建议1us,延时过低芯片没有响应
        gpio_set_level(SCK, 1);
        esp_rom_delay_us(1);
    }

    gpio_set_level(RCK, 0);
    esp_rom_delay_us(1);
    gpio_set_level(RCK, 1);
    esp_rom_delay_us(1);
}
