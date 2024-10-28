#include "htr3218.h"

#define HTR3218_ADDR 0x3C

// 使能 htr3218 输出
uint8_t write_buf4[2] = {0x4A, 0x00};
uint8_t write_buf5[2] = {0x25, 0x11};

extern i2c_master_bus_handle_t bus_handle;
i2c_master_dev_handle_t htr3218_handle;

i2c_device_config_t htr3218_cfg =
    {
        .dev_addr_length = I2C_ADDR_BIT_7,
        .device_address = HTR3218_ADDR,
        .scl_speed_hz = 100000,
};

void htr3218_init(void)
{
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &htr3218_cfg, &htr3218_handle));
    Htr3218_reset();
}

/**
 * led对应htr3218的outx引脚
 *
 *
 *            PM1.0(22)                       PM2.5(23)               PM10 (24)      TVOC(25)
 *
 *
 *
 *            led(15)                         led(8)                     led(1)
 *                                                                                                   wifi(26)
 *
 *    led(20)         led(16)         led(13)         led(9)     led(6)            led(2)           hins(27)
 *             led(21)                         led(14)                    led(7)
 *
 *    led(19)         led(17)         led(12)         led(10)     led(5)          led(3)
 *            led(18)                         led(11)                    led(4)                     filter(28)
 *
 *      RGB-red(29)                  RGB-green(30) RGB-blue(31)
 *
 * 程序上可以分区显示
 *可以通过输入的数字,来选择哪几个灯亮
 *
 */

void Htr3218_reset(void)
{
    uint8_t write_buf5[2] = {0x4F, 0x00}; // htr3218复位命令
    i2c_master_transmit(htr3218_handle, write_buf5, sizeof(write_buf5), 100 / portTICK_PERIOD_MS);
}

// if want change brigtness ,insert variable my_brightness
void turn_on_RGB(uint16_t PM_value, uint8_t mybrightness)
{
    uint8_t RGB_config[8] = {0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    if (PM_value == 1)
    {
        RGB_config[2] = mybrightness;
        RGB_config[7] = 0x00;
    }
    if (PM_value == 2)
    {
        RGB_config[2] = 0x00;
        RGB_config[7] = mybrightness;
    }

    uint8_t write_buf0[2] = {0x00, 0x01};
    ESP_ERROR_CHECK(i2c_master_transmit(htr3218_handle, write_buf0, sizeof(write_buf0), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS));

    // 选择通道
    uint8_t write_buf1[] = {0x2F, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
                            0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}; // constant write data in
    i2c_master_transmit(htr3218_handle, write_buf1, sizeof(write_buf1), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);

    i2c_master_transmit(htr3218_handle, RGB_config, sizeof(RGB_config), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);

    i2c_master_transmit(htr3218_handle, write_buf4, sizeof(write_buf4), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);

    i2c_master_transmit(htr3218_handle, write_buf5, sizeof(write_buf5), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
}

void htr3218_all_led_on(void)
{
    uint8_t RGB_config[19] = {0x0A, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    uint8_t write_buf0[2] = {0x00, 0x01};
    ESP_ERROR_CHECK(i2c_master_transmit(htr3218_handle, write_buf0, sizeof(write_buf0), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS));

    // 选择通道
    uint8_t write_buf1[] = {0x2F, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
                            0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}; // constant write data in
    i2c_master_transmit(htr3218_handle, write_buf1, sizeof(write_buf1), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);

    i2c_master_transmit(htr3218_handle, RGB_config, sizeof(RGB_config), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);

    i2c_master_transmit(htr3218_handle, write_buf4, sizeof(write_buf4), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);

    i2c_master_transmit(htr3218_handle, write_buf5, sizeof(write_buf5), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
}