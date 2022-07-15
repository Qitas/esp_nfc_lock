#ifndef INC_ESP_PORT_H_
#define INC_ESP_PORT_H_

#include <stdint.h>

#include "esp_wifi.h"
#include "driver/i2s.h"
#include "driver/uart.h"
#include "driver/spi_master.h"


#define PN532_I2C_PORT          0
#define UART_NUM                UART_NUM_1
#define SPI_HOST_NUM            SPI2_HOST

#define SPI_TRANS_MAX_SZ          (1000*32)   
extern spi_device_handle_t spi_host;


extern void wifi_init(void);
extern void spi_host_init(void);
extern wifi_config_t *wifi_get_conf(void);

extern char *wifi_get_hostname(void);
extern char *wifi_get_mac_string(void);
extern uint8_t *wifi_get_mac_address(void);
extern void i2s_output_init(void);
extern void i2s_output_deinit(void);
extern void i2s_output_set_sample_rate(unsigned int sample_rate);



#endif /* INC_ESP_PORT_H_ */
