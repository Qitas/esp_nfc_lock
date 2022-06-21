
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "bsp/port.h"
#include "sdkconfig.h"

#define TAG "pn532"

// #define CONFIG_PN532_TX_PIN 16
// #define CONFIG_PN532_RX_PIN 15



#define RESET_PIN           CONFIG_PN532_RST_PIN
#define IRQ_PIN             CONFIG_PN532_IRQ_PIN
#define PN532_RX            CONFIG_PN532_RX_PIN
#define PN532_TX            CONFIG_PN532_TX_PIN
#define SDA_PIN             CONFIG_PN532_TX_PIN
#define SCL_PIN             CONFIG_PN532_RX_PIN
#define SCL_PIN             CONFIG_PN532_RX_PIN



void pn532_power_reset(uint8_t val)
{
    gpio_set_level(CONFIG_PN532_RST_PIN, val);
}

static void pn532_reset(void)
{
  gpio_set_level (RESET_PIN, 1);
  gpio_set_level (RESET_PIN, 0);
  vTaskDelay (400 / portTICK_PERIOD_MS);
  gpio_set_level (RESET_PIN, 1);
  vTaskDelay (10 / portTICK_PERIOD_MS);
}


void pn532_uart_init(void)
{
    gpio_config_t rst_conf = {
        .pin_bit_mask = BIT64(RESET_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = false,
        .pull_down_en = false,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&rst_conf);

    
    uart_config_t uart_conf = {
       .baud_rate = 115200,
       .data_bits = UART_DATA_8_BITS,
       .parity = UART_PARITY_DISABLE,
       .stop_bits = UART_STOP_BITS_1,
       .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    ESP_ERROR_CHECK(uart_param_config(UART_NUM, &uart_conf));
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM, 256, 256, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM, PN532_RX, PN532_TX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    ESP_ERROR_CHECK(uart_flush_input(UART_NUM));
    pn532_reset();
    ESP_LOGI(TAG, "rst: %d, irq: %d, txd: %d, rxd: %d",RESET_PIN,IRQ_PIN,PN532_RX,PN532_TX);
}


// bool pn532_i2c_init(void)
// {
//   SCL_PIN = scl;
//   SDA_PIN = sda;
//   IRQ_PIN = irq;
//   PN532_I2C_PORT = i2c_port_number;
//   uint64_t pintBitMask = 0;
//   // uint64_t pintBitMask = ((1ULL) << RESET_PIN);
//   gpio_config_t io_conf;

//   pintBitMask = ((1ULL) << IRQ_PIN);
//   //Lets configure GPIO PIN for IRQ
// #ifdef ENABLE_IRQ_ISR
//   io_conf.intr_type = GPIO_INTR_NEGEDGE;
// #endif
//   //set as output mode
//   io_conf.mode = GPIO_MODE_INPUT;
//   //bit mask of the pins that you want to set,e.g.GPIO18/19
//   io_conf.pin_bit_mask = pintBitMask;
//   //disable pull-down mode
//   io_conf.pull_down_en = 0;
//   //enable pull-up mode
//   io_conf.pull_up_en = 0;
//   //configure GPIO with the given settings
//   if (gpio_config (&io_conf) != ESP_OK) return false;
//   // Reset the PN532
//   // pn532_reset ();
//   if (pn532_irq_queue != NULL) vQueueDelete (pn532_irq_queue);
//   pn532_irq_queue = xQueueCreate(1, sizeof(uint32_t));
//   gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
//   gpio_isr_handler_add(IRQ_PIN, pn532_irq_callback, (void* )IRQ_PIN);

//   i2c_config_t conf = {0};
//   //Open the I2C Bus
//   conf.mode = I2C_MODE_MASTER;
//   conf.sda_io_num = SDA_PIN;
//   conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
//   conf.scl_io_num = SCL_PIN;
//   conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
//   conf.master.clk_speed = 10000;

//   if (i2c_param_config (PN532_I2C_PORT, &conf) != ESP_OK) return false;
//   if (i2c_driver_install (PN532_I2C_PORT, conf.mode, 0, 0, 0) != ESP_OK) return false;
//   //Needed due to long wake up procedure on the first command on i2c bus. May be decreased
//   if (i2c_set_timeout (PN532_I2C_PORT, 100000) != ESP_OK) return false;

//   return true;
// }