
#include "core/os.h"
#include "core/app.h"
#include "port/nvs.h"
#include "port/spi.h"
#include "port/i2s.h"
#include "port/wifi.h"
#include "port/uart.h"
#include "bsp/pn532.h"

#include "user/ntp.h"
#include "user/led.h"
#include "user/gui.h"
#include "user/key.h"
#include "user/nfc_app.h"
#include "user/http_app.h"
#include "user/audio_player.h"

/*******************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
********************************************************************************/
static void core_init(void)
{
    app_print_info();

    os_init();
}

static void chip_init(void)
{
    nvs_init();

    wifi_init();

    uart_init();

#ifdef CONFIG_ENABLE_AUDIO_PROMPT
    i2s_output_init();
#endif

#ifdef CONFIG_ENABLE_GUI
    spi_host_init();
#endif
}

static void rfid_init(void)
{
    pn532_init();
}

static void user_init(void)
{
    ntp_init();
    nfc_app_init();
    http_app_init();
#ifdef CONFIG_ENABLE_SC_KEY
    key_init();
#endif

#ifdef CONFIG_ENABLE_LED
    led_init();
#endif

#ifdef CONFIG_ENABLE_GUI
    gui_init();
#endif

#ifdef CONFIG_ENABLE_AUDIO_PROMPT
    audio_player_init();
#endif
}
/*******************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
********************************************************************************/
void app_main(void)
{
    core_init();
    chip_init();
    rfid_init();
    user_init();
}
