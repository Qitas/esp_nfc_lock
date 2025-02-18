#include <string.h>

#include "esp_log.h"

#include "nfc/nfc.h"
#include "sdkconfig.h"
#include "core/os.h"
#include "bsp/pn532.h"

#include "user/gui.h"
#include "user/ntp.h"
#include "user/led.h"
#include "user/nfc_app.h"
#include "user/audio_player.h"
#include "user/http_app_token.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "bsp/port.h"

#define TAG "nfc_app"

#define RX_FRAME_PRFX "FF55AA55AA"

#define RX_FRAME_PRFX_LEN (10)
#define RX_FRAME_DATA_LEN (32)

#define RX_FRAME_LEN    (RX_FRAME_PRFX_LEN + RX_FRAME_DATA_LEN)
#define TX_FRAME_LEN    (10)

static uint8_t rx_data[RX_FRAME_LEN + 1] = {0x00};
static uint8_t tx_data[TX_FRAME_LEN + 1] = {0x00, 0xA4, 0x04, 0x00, 0x05};

static nfc_app_mode_t nfc_app_mode = NFC_APP_MODE_IDX_OFF;

static int char2int(char ch)
{
    if (ch >= '0' && ch <= '9') {
        return ch - '0';
    } else if (ch >= 'A' && ch <= 'F') {
        return ch - 'A' + 10;
    } else if (ch >= 'a' && ch <= 'f') {
        return ch - 'a' + 10;
    } else {
        return 0;
    }
}

static void str2byte(const char *str, char *byte)
{
    for (; str[0] && str[1]; str += 2) {
        *(byte++) = char2int(str[0]) * 16 + char2int(str[1]);
    }
}

static void nfc_app_task_handle(void *pvParameter)
{
    nfc_target nt;
    nfc_device *pnd;
    nfc_context *context;
    nfc_modulation nm = {
        .nmt = NMT_ISO14443A,
        .nbr = NBR_106
    };
    portTickType xLastWakeTime;
    str2byte(RX_FRAME_PRFX, (char *)tx_data + 5);
    nfc_init(&context);
    if (context == NULL) {
        ESP_LOGE(TAG, "failed to init libnfc");
        goto err;
    }
    while ((pnd = nfc_open(context, "pn532_uart:uart1:115200")) == NULL) {    
        pn532_power_reset(0);  //通过电源控制管脚控制模块的复位
        vTaskDelay(400 / portTICK_RATE_MS);
        pn532_power_reset(1);
        ESP_LOGE(TAG, "device power reset");
        vTaskDelay(100 / portTICK_RATE_MS);
    } 
    while (true) {
        // xEventGroupWaitBits(
        //     user_event_group,
        //     NFC_APP_RUN_BIT,
        //     pdFALSE,
        //     pdFALSE,
        //     portMAX_DELAY
        // );
        xLastWakeTime = xTaskGetTickCount();
        while ((pnd = nfc_open(context, "pn532_uart:uart1:115200")) == NULL) {
            ESP_LOGE(TAG, "device hard reset");
            pn532_power_reset(0);  //通过电源控制管脚控制模块的复位
            vTaskDelay(400 / portTICK_RATE_MS);
            pn532_power_reset(1);
            vTaskDelay(100 / portTICK_RATE_MS);
        }
        int res = 0;
        if (nfc_initiator_init(pnd) >= 0) {
            if (nfc_initiator_select_passive_target(pnd, nm, NULL, 0, &nt) >= 0) {
                ESP_LOGI(TAG, "abtUid %d：%d %d %d %d",nt.nti.nai.szUidLen,nt.nti.nai.abtUid[0],nt.nti.nai.abtUid[1],nt.nti.nai.abtUid[2],nt.nti.nai.abtUid[3]);
                // if ((res = nfc_initiator_transceive_bytes(pnd, tx_data, TX_FRAME_LEN, rx_data, RX_FRAME_LEN, -1)) >= 0) {
                //     rx_data[res] = 0x00;
                // } 
                // else {
                //     ESP_LOGW(TAG, "failed to transceive bytes");
                // }
            } 
            // else {
            //     ESP_LOGI(TAG, "waiting for NFC target....");
            // }
        } 
        else {
            ESP_LOGE(TAG, "failed to init device");
        }
        nfc_close(pnd);
        if (res > 0) {
            if (strstr((char *)rx_data, RX_FRAME_PRFX) != NULL &&
                strlen((char *)rx_data + RX_FRAME_PRFX_LEN) == RX_FRAME_DATA_LEN) {
                ESP_LOGW(TAG, "token: %32s", rx_data + RX_FRAME_PRFX_LEN);
#ifdef CONFIG_ENABLE_AUDIO_PROMPT
                audio_player_play_file(MP3_FILE_IDX_NOTIFY);
#endif
                http_app_verify_token((char *)rx_data + RX_FRAME_PRFX_LEN);
            } else {
                ESP_LOGW(TAG, "unexpected frame length: %d %d %d", strlen((char *)rx_data),rx_data[0],rx_data[1]);
            }
        }
        vTaskDelayUntil(&xLastWakeTime, 500 / portTICK_RATE_MS);
    }

err:
    nfc_exit(context);
    ESP_LOGE(TAG, "unrecoverable error");
    os_pwr_reset_wait(OS_PWR_DUMMY_BIT);
    vTaskDelete(NULL);
}


void nfc_app_set_mode(nfc_app_mode_t idx)
{
    nfc_app_mode = idx;
    if (nfc_app_mode == NFC_APP_MODE_IDX_ON) {
        pn532_power_reset(1);
        vTaskDelay(100 / portTICK_RATE_MS);
        xEventGroupSetBits(user_event_group, NFC_APP_RUN_BIT);
    } else {
        xEventGroupClearBits(user_event_group, NFC_APP_RUN_BIT);
        pn532_power_reset(0);
    }
}

nfc_app_mode_t nfc_app_get_mode(void)
{
    return nfc_app_mode;
}

#define ENABLE_IRQ_ISR          1
#define IRQ_PIN                 CONFIG_PN532_IRQ_PIN
#define ESP_INTR_FLAG_DEFAULT   0

static xQueueHandle pn532_irq_queue = NULL;

static void IRAM_ATTR pn532_irq_callback(void *arg)
{
    // ESP_LOGE(TAG, "pn532_irq_callback");
    uint32_t gpio_num =(uint32_t)arg;
    xQueueSendFromISR(pn532_irq_queue, &gpio_num, NULL);
}

void nfc_app_init(void)
{
    pn532_uart_init();
    xTaskCreatePinnedToCore(nfc_app_task_handle, "nfcAppT", 5120*3, NULL, 5, NULL, 0);
#ifdef ENABLE_IRQ_ISR
    gpio_config_t irq_conf = {
        .pin_bit_mask = BIT64(IRQ_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = false,
        .pull_down_en = false,
        .intr_type = GPIO_INTR_NEGEDGE
    };
    gpio_config(&irq_conf);
    if (pn532_irq_queue != NULL) vQueueDelete(pn532_irq_queue);
    pn532_irq_queue = xQueueCreate(1, sizeof(uint32_t));
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    gpio_isr_handler_add(IRQ_PIN, pn532_irq_callback, (void* )IRQ_PIN);
    while (true) {
        uint32_t io_num = 0;
        xQueueReceive(pn532_irq_queue, &io_num, portMAX_DELAY);
        ESP_LOGW (TAG, "xQueueReceive %d!",io_num);
    }
    #endif
}
