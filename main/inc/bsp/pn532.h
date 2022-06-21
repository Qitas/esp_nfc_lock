
#ifndef INC_BOARD_PN532_H_
#define INC_BOARD_PN532_H_

#include <stdint.h>

extern void pn532_power_reset(uint8_t val);

extern void pn532_uart_init(void);

#endif /* INC_BOARD_PN532_H_ */
