
#include "core/os.h"
#include "core/app.h"
#include "bsp/port.h"
#include "bsp/pn532.h"
#include "user/ntp.h"
#include "user/led.h"
#include "user/gui.h"
#include "user/key.h"
#include "user/nfc_app.h"
#include "user/http_app.h"
#include "user/audio_player.h"

#include "esp_log.h"
#include "nvs_flash.h"

#define TAG     "main"
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
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    ESP_LOGI(TAG, "initialized.");


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
