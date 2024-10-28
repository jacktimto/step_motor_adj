#include "eeprom.h"

#define SCL 19
#define SDA 18
#define IIC_PORT 0
extern uint16_t target_motor_speed;
#define I2C_timeout 100

extern i2c_master_bus_handle_t bus_handle;
i2c_master_dev_handle_t dev_handle;
uint8_t Tx_buffer[2];
uint8_t Rx_buffer[32];

/**
 * write Specify address
 * pdata 需要写入的数组,写入的数组首个元素要赋值给wo
 * len   数组长度
 */
void eeprom_write(uint16_t device_addr, uint8_t *pdata, uint8_t len)
{
    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = (0xA0 + (uint8_t)((device_addr & 0x0700) >> 8)) >> 1,
        .scl_speed_hz = 100000,
    };
    *pdata = (uint8_t)(device_addr & 0x00FF);
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_cfg, &dev_handle));
    ESP_ERROR_CHECK(i2c_master_transmit(dev_handle, pdata, len, I2C_timeout));
}
uint16_t eeprom_read(uint16_t device_addr, uint8_t len)
{
    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = (0xA0 + (uint8_t)((device_addr & 0x0700) >> 8)) >> 1,
        .scl_speed_hz = 100000,
    };

    i2c_master_dev_handle_t dev_handle;
    Tx_buffer[0] = (uint8_t)(device_addr & 0x00FF);
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_cfg, &dev_handle));
    ESP_ERROR_CHECK(i2c_master_transmit_receive(dev_handle, &Tx_buffer[0], 1, Rx_buffer, len, I2C_timeout));
    // for (uint8_t i = 0; i < len; i++)
    // {
    //     printf("Rx_buffer[%d]:0x%02x\n",i,Rx_buffer[i]);
    // }
    return Rx_buffer[0] * 256 + Rx_buffer[1];
}

// void eeprom_period_write(void *parameter)
// {
//     while (1)
//     {
//         eeprom_write(0x0001, (uint8_t)(target_motor_speed >> 8));
//         vTaskDelay(100 / portTICK_PERIOD_MS);
//         eeprom_write(0x0002, (uint8_t)(target_motor_speed & 0x00FF));
//         vTaskDelay(10000 / portTICK_PERIOD_MS);
//     }
// }