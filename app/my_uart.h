#ifndef UART_H_
#define UART_H_

/**
 * esp32与BYD芯片通讯
 * 实现单个按键按下,按键长按的识别,组合键的识别
 */

#include "driver/uart.h"
#include "esp_log.h"

// define uart1
static const int RX_BUF_SIZE = 1024;

#define TXD_PIN (25)  
#define RXD_PIN (26) 
#define TX_TO_BYD (22)    
#define RX_FROM_BYD (23)   

void uart_init(void);
int8_t uart_checksum(uint8_t *rxdata);
void pm1003_read(void * paramter);
void esp32_write_to_byd(void *paramter);


#endif