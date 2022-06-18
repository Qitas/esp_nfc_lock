
#include "esp_log.h"

#include "port/uart.h"

void uart_init(void)
{
    uart_config_t uart_conf = {
       .baud_rate = 115200,
       .data_bits = UART_DATA_8_BITS,
       .parity = UART_PARITY_DISABLE,
       .stop_bits = UART_STOP_BITS_1,
       .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    ESP_ERROR_CHECK(uart_param_config(UART_NUM, &uart_conf));
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM, 256, 256, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM, CONFIG_PN532_RX_PIN, CONFIG_PN532_TX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    ESP_ERROR_CHECK(uart_flush_input(UART_NUM));

    ESP_LOGI(UART_TAG, "initialized, txd: %d, rxd: %d", CONFIG_PN532_RX_PIN, CONFIG_PN532_TX_PIN);
}
