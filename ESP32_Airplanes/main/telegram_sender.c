#include "esp_http_client.h"
#include "esp_crt_bundle.h"
#include "esp_log.h"
#include "project_config.h"

static const char *TAG = "TELEGRAM";

void send_telegram_message(const char* message) {
    char url[256];
    
    snprintf(url, sizeof(url), "https://api.telegram.org/bot%s/sendMessage", TELEGRAM_TOKEN);

    // JSON payload: {"chat_id": "123", "text": "Hello!"}
    char post_data[512];
    snprintf(post_data, sizeof(post_data), "{\"chat_id\": \"%s\", \"text\": \"%s\"}", 
             TELEGRAM_CHAT_ID, message);

    esp_http_client_config_t config = {
        .url = url,
        .method = HTTP_METHOD_POST,
        .crt_bundle_attach = esp_crt_bundle_attach,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    
    esp_http_client_set_post_field(client, post_data, strlen(post_data));
    esp_http_client_set_header(client, "Content-Type", "application/json");

    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK) {
        ESP_LOGI(TAG, "Message sent! Status = %d", esp_http_client_get_status_code(client));
    } else {
        ESP_LOGE(TAG, "Failed to send message: %s", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);
}