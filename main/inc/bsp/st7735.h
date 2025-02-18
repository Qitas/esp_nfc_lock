
#ifndef INC_BOARD_ST7735_H_
#define INC_BOARD_ST7735_H_

#include <stdint.h>

#include "bsp/port.h"

#define ST7735_SCREEN_WIDTH  80
#define ST7735_SCREEN_HEIGHT 160

extern void st7735_init_board(void);

extern void st7735_set_backlight(uint8_t val);
extern void st7735_setpin_dc(spi_transaction_t *);
extern void st7735_setpin_reset(uint8_t val);

extern void st7735_write_cmd(uint8_t cmd);
extern void st7735_write_data(uint8_t data);
extern void st7735_write_buff(uint8_t *buff, uint32_t n);
extern void st7735_refresh_gram(uint8_t *gram);

#endif /* INC_BOARD_ST7735_H_ */
