#include "nvs_flash.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "wifi_connector.h"
#include "airplane_fetcher.h"
#include "flight_manager.h"


static const char *TAG = "MAIN";

void app_main(void) {

    // 1. Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // 2. Connect to Wi-Fi
    if (wifi_init_sta() == ESP_OK) {
        ESP_LOGI(TAG, "Internet Ready. Ready for Step 2!");
        // Here is where you will eventually call your airplane data task
        xTaskCreate(airplane_fetcher_task, "airplane_fetcher_task", 8192, NULL, 5, NULL);
    } else {
        ESP_LOGE(TAG, "Failed to connect. System Halted.");
    }

}