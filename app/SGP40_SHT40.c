#include "SGP40_SHT40.h"

#define SHT40_ADDR 0x44
#define SGP40_ADDR 0x59

uint8_t SHT40_RXBUFFER[6] = {0};
uint8_t SHT40_RESET[1] = {0x94};
uint8_t SHT40_READ[1] = {0xFD};

float temperature = 0.00f, humidity = 0.00f;

uint8_t temp[2] = {0};

uint8_t SGP40_TX_BUFF[8] = {0x26, 0x0F, 0x80, 0x00, 0xA2, 0x66, 0x66, 0x93}; // sgp40 read command
uint8_t SGP40_RX_BUFF[3] = {0};                                              // save return data from sgp40
uint8_t SGP40_REST[2] = {0x00, 0x06};

int32_t voc_index;
int32_t temperature_celsius;
int32_t relative_humidity_percent;

extern i2c_master_bus_handle_t bus_handle;
VocAlgorithmParams voc_algorithm_params;
i2c_master_dev_handle_t SGP40_handle, SHT40_handle;

i2c_device_config_t SGP40_cfg =
    {
        .dev_addr_length = I2C_ADDR_BIT_7,
        .device_address = SGP40_ADDR,
        .scl_speed_hz = 100000,
};

i2c_device_config_t SHT40_cfg =
    {
        .dev_addr_length = I2C_ADDR_BIT_7,
        .device_address = SHT40_ADDR,
        .scl_speed_hz = 100000,
};

// add cfg to i2c
void SGP40_SHT40_init(void)
{
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &SGP40_cfg, &SGP40_handle));
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &SHT40_cfg, &SHT40_handle));
}

// CRC校验
uint8_t CalcCrc(int32_t temp_data)
{
    uint8_t crc = 0xFF;
    uint8_t data[2] = {0};
    data[0] = (temp_data & 0xFF00) >> 8;
    data[1] = (uint8_t)(temp_data & 0x00FF);

    for (int i = 0; i < 2; i++)
    {
        crc ^= data[i];
        for (uint8_t bit = 8; bit > 0; --bit)
        {
            if (crc & 0x80)
            {
                crc = (crc << 1) ^ 0x31u;
            }
            else
            {
                crc = (crc << 1);
            }
        }
    }
    return crc;
}

// i2c read
void SGP40_SHT40_read(void *parameter)
{

    // int32_t voc_index;
    int32_t temperature_celsius;
    int32_t relative_humidity_percent;
    uint16_t voc_raw;

    // sht40 reset,sgp40 reset
    // i2c_master_transmit(SGP40_handle, SGP40_REST, sizeof(SGP40_REST), -1);
    // vTaskDelay(500 / portTICK_PERIOD_MS);
    // i2c_master_transmit(SHT40_handle, SHT40_RESET, sizeof(SHT40_RESET), -1);
    // vTaskDelay(500 / portTICK_PERIOD_MS);

    while (1)
    {
        // sht40 read
        i2c_master_transmit(SHT40_handle, SHT40_READ, sizeof(SHT40_READ), -1);
        vTaskDelay(50 / portTICK_PERIOD_MS);
        i2c_master_receive(SHT40_handle, SHT40_RXBUFFER, sizeof(SHT40_RXBUFFER), -1);
        temperature_celsius = SHT40_RXBUFFER[0] * 256 + SHT40_RXBUFFER[1];
        relative_humidity_percent = SHT40_RXBUFFER[3] * 256 + SHT40_RXBUFFER[4];
        temperature = (-45 + 175 * (temperature_celsius) / (pow(2, 16) - 1));
        humidity = (-6 + 125 * (relative_humidity_percent) / (pow(2, 16) - 1));

        // sgp40 read with temp humidity
        // SGP40_TX_BUFF[2] = SHT40_RXBUFFER[3];
        // SGP40_TX_BUFF[3] = SHT40_RXBUFFER[4];

        // SGP40_TX_BUFF[4] = CalcCrc(relative_humidity_percent);
        // SGP40_TX_BUFF[5] = SHT40_RXBUFFER[0];
        // SGP40_TX_BUFF[6] = SHT40_RXBUFFER[1];
        // SGP40_TX_BUFF[7] = CalcCrc(temperature_celsius);

        // i2c_master_transmit(SGP40_handle, SGP40_TX_BUFF, sizeof(SGP40_TX_BUFF), -1);
        // vTaskDelay(50 / portTICK_PERIOD_MS);
        // i2c_master_receive(SGP40_handle, SGP40_RX_BUFF, sizeof(SGP40_RX_BUFF), -1);
        // voc_raw = SGP40_RX_BUFF[0] * 256 + SGP40_RX_BUFF[1];

        // VocAlgorithm_process(&voc_algorithm_params, voc_raw, &voc_index);
        // ESP_LOGI("SHT40 Read", "temperature_celsius:%ld,relative_humidity_percent:%ld", temperature_celsius, relative_humidity_percent);
        ESP_LOGI("SHT40 Read", "temperature:%0.2f℃,relative_humidity:%0.2f%%", temperature, humidity);
        // ESP_LOGI("SGP40 Read", "TVOC RAW:%d,voc_index:%ld", voc_raw, voc_index);
        vTaskDelay(950 / portTICK_PERIOD_MS);
    }
}

// // SHT用的是CRC校验
// int SHT40_CHECKSUM(uint8_t *SHT40_RXBUFF)
// {
//     uint8_t temp_data[2] = {SHT40_RXBUFF[0], SHT40_RXBUFF[1]};
//     uint8_t humi_data[2] = {SHT40_RXBUFF[3], SHT40_RXBUFF[4]};
//     if ((SHT40_RXBUFF[2] == CalcCrc(temp_data)) && (SHT40_RXBUFF[5] == CalcCrc(humi_data)))
//     {
//         return 0;
//     }
//     else
//     {
//         return -1;
//     }
// }

// // SGP40用的是CRC校验
// int SGP40_CHECKSUM(uint8_t *SGP40_RXBUFF)
// {
//     uint8_t sgp40_data[2] = {SGP40_RXBUFF[0], SGP40_RXBUFF[1]};
//     if (SGP40_RXBUFF[2] == CalcCrc(sgp40_data))
//     {
//         return 0;
//     }
//     else
//     {
//         return -1;
//     }
// }
