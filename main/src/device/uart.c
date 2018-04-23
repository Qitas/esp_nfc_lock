/*
 * uart.c
 *
 *  Created on: 2018-02-10 16:09
 *      Author: Jack Chen <redchenjs@live.com>
 */

#include "esp_log.h"
#include "driver/uart.h"
#include "device/uart.h"

#define EX_UART_TXD  CONFIG_UART_TXD_PIN
#define EX_UART_RXD  CONFIG_UART_RXD_PIN

#define BUF_SIZE (128)

static const char *TAG = "uart-1";

void uart1_init()
{
    uart_config_t uart_config = {
       .baud_rate = 115200,
       .data_bits = UART_DATA_8_BITS,
       .parity = UART_PARITY_DISABLE,
       .stop_bits = UART_STOP_BITS_1,
       .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
       .rx_flow_ctrl_thresh = 16,
    };
    // Set UART parameters
    uart_param_config(EX_UART_NUM, &uart_config);
    // Set UART log level
    esp_log_level_set(TAG, ESP_LOG_INFO);
    // Install UART driver, and get the queue
    uart_driver_install(EX_UART_NUM, BUF_SIZE * 2, BUF_SIZE * 2, 0, NULL, 0);
    // Set UART pins
    uart_set_pin(EX_UART_NUM, EX_UART_TXD, EX_UART_RXD, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    // Flush UART RX Buffer
    uart_flush_input(EX_UART_NUM);
}
