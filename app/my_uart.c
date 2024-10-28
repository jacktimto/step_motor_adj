#include "my_uart.h"
#include "htr3236.h"
#include "eeprom.h"
#include "step_motor.h"

#define BYD_VERSION 5 // 5代表BYD程序不带NTC返回参数
extern step_motor_config_t step1;

uint8_t rx_data[20];

QueueHandle_t uart_queue;
uint8_t PM25_TxBuffer[5] = {0x11, 0x02, 0x0B, 0x01, 0xE1};
uint8_t BYD_VERSION_CHECK[5] = {0xA5, 0x05, 0x34, 0xDE, 0x55};
uint8_t BYD_TOUCH_READ[5] = {0xA5, 0x05, 0x33, 0xDD, 0x55};
uint8_t BYD_KEY[16];

#define baudrate_9600 9600
#define baudrate_115200 115200
extern uint16_t target_motor_speed;

// uart init
void uart_init(void)
{
    const uart_config_t uart_config = {
        .baud_rate = baudrate_9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    const uart_config_t uart_config_byd = {
        .baud_rate = baudrate_115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    uart_param_config(UART_NUM_1, &uart_config);
    uart_param_config(UART_NUM_2, &uart_config_byd); //
    uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_set_pin(UART_NUM_2, TX_TO_BYD, RX_FROM_BYD, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_driver_install(UART_NUM_2, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
}

/**
 * 对应sscom的 0-ADD8校验
 * pm1003 checksum
 * 0,校验正确;-1,校验错误;-2,帧头错误
 */
int8_t uart_checksum(uint8_t *rxdata)
{
    uint8_t temp_checksum = 0;
    uint16_t temp_sum = 0;
    if (rx_data[0] == 0x16)
    {
        for (int i = 0; i < 20; i++)
        {
            temp_sum = temp_sum + rx_data[i];
        }
        temp_checksum = 256 - temp_sum;
        if (rx_data[4] == temp_checksum)
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -2;
    }
}

/**
 * byd_checksum:一个字节，从Start一直累加到Data n的累加和取低8位。
 * return 0,校验ok, return -1,帧头检验失败,return -2,checksum校验错误
 */
int8_t byd_checksum(uint8_t *rxdata)
{
    if (rxdata[0] == 0x5A)
    {

        if (BYD_VERSION == 5)
        {
            uint8_t temp_sum = rxdata[0] + rxdata[1] + rxdata[2] + rxdata[3] + rxdata[4];
            if (temp_sum == rxdata[5])
            {
                temp_sum = 0;
                return 0;
            }
            else
            {
                temp_sum = 0;
                return -2;
            }
        }
        else
        {
            uint8_t temp_sum = rxdata[0] + rxdata[1] + rxdata[2] + rxdata[3] + rxdata[4] + rxdata[5] + rxdata[6];
            if (temp_sum == rxdata[7])
            {
                temp_sum = 0;
                return 0;
            }
            else
            {
                temp_sum = 0;
                return -2;
            }
        }
    }
    else
    {
        return -1;
    }
}

void pm1003_read(void *paramter)
{
    vTaskDelay(pdMS_TO_TICKS(2000));
    while (1)
    {
        uart_write_bytes(UART_NUM_1, PM25_TxBuffer, sizeof(PM25_TxBuffer));
        uart_read_bytes(UART_NUM_1, rx_data, sizeof(rx_data), pdMS_TO_TICKS(100));
        uart_flush(UART_NUM_1);
        ESP_LOGI("PM2.5_read", "PM2.5:%d", rx_data[5] * 256 + rx_data[6]);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void esp32_write_to_byd(void *paramter)
{
    uint8_t byd_rx_buff[18];
    while (1)
    {
        uart_flush(UART_NUM_2); //
        uart_write_bytes(UART_NUM_2, BYD_TOUCH_READ, sizeof(BYD_TOUCH_READ));
        uart_read_bytes(UART_NUM_2, byd_rx_buff, sizeof(byd_rx_buff), pdMS_TO_TICKS(10));
        // for (int i = 0; i < 9; i++)
        // {
        //     ESP_LOGI("test", "rxbuff[%d]:0x%02X", i, byd_rx_buff[i]);
        // }

        if (byd_checksum(byd_rx_buff) == 0x00)
        {
            for (int i = 0; i < 16; i++)
            {
                if ((((byd_rx_buff[4] << 8) + byd_rx_buff[3]) >> i) & 0x01)
                {
                    BYD_KEY[i] = 0x01;
                }
            }
            if (BYD_KEY[0] == 0x01) // fan speed button,减速频率
            {
                BYD_KEY[0] = 0x00;
                step1.speed += 10;
                if (step1.speed > 1900)
                {
                    step1.speed = 1900;
                }
                Htr3236_write(step1.speed, 0x10);
            }
            if (BYD_KEY[1] == 0x01) // auto button ,加速频率
            {
                BYD_KEY[1] = 0x00;

                step1.speed -= 10;
                if(step1.speed > 3000)
                {
                    esp_restart();
                }
                Htr3236_write(step1.speed, 0x10);
            }

            if (BYD_KEY[2] == 0x01) // auto button ,加速频率
            {
                BYD_KEY[2] = 0x00;
                if (step1.angle == 0x0001)
                {
                    step1.angle = -1;
                }
                else
                {
                    step1.angle = 0x0001;
                }
                Htr3236_write(step1.speed, 0x10);
                vTaskDelay(pdMS_TO_TICKS(300));
                ESP_LOGI("test", "button eco pressed");
            }
        }

        memset(byd_rx_buff, 0x00, sizeof(byd_rx_buff)); // clear 已读的数据
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}