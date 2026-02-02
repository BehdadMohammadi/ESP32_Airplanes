#include "airplane_fetcher.h"
#include "esp_http_client.h"
#include "esp_crt_bundle.h"
#include "esp_log.h"
#include "cJSON.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "flight_manager.h"

static const char *TAG = "PLANE_FETCH";

// Europe/London area bounding box
// #define LAMIN "51.2"
// #define LAMAX "51.7"
// #define LOMIN "-0.5"
// #define LOMAX "0.3"

// Vacouver, Canada area bounding box
#define LAMIN "49.0"
#define LAMAX "49.5"
#define LOMIN "-123.5"
#define LOMAX "-122.5"

#define OPENSKY_URL "https://opensky-network.org/api/states/all?lamin=" LAMIN "&lomin=" LOMIN "&lamax=" LAMAX "&lomax=" LOMAX

char *response_buffer = NULL;
int response_len = 0;

esp_err_t _http_event_handler(esp_http_client_event_t *evt) {
    if (evt->event_id == HTTP_EVENT_ON_DATA) {
        response_buffer = realloc(response_buffer, response_len + evt->data_len + 1);
        memcpy(response_buffer + response_len, evt->data, evt->data_len);
        response_len += evt->data_len;
        response_buffer[response_len] = 0;
    }
    return ESP_OK;
}

// void parse_and_print_flights(const char *json_str) {
//     cJSON *root = cJSON_Parse(json_str);
//     if (root == NULL) {
//         ESP_LOGE(TAG, "Failed to parse JSON");
//         return;
//     }

//     cJSON *states = cJSON_GetObjectItem(root, "states");
//     if (!cJSON_IsArray(states)) {
//         ESP_LOGI(TAG, "No planes found in this area.");
//         cJSON_Delete(root);
//         return;
//     }

//     int total_planes = cJSON_GetArraySize(states);
//     int to_print = (total_planes > 10) ? 10 : total_planes;

//     ESP_LOGI(TAG, "Found %d planes. Printing first %d:", total_planes, to_print);

//     for (int i = 0; i < to_print; i++) {
//         cJSON *plane = cJSON_GetArrayItem(states, i);
        
//         // OpenSky format: [0]=icao24, [1]=callsign, [5]=long, [6]=lat
//         char *icao = cJSON_GetArrayItem(plane, 0)->valuestring;
//         char *callsign = cJSON_GetArrayItem(plane, 1)->valuestring;
//         double lon = cJSON_GetArrayItem(plane, 5)->valuedouble;
//         double lat = cJSON_GetArrayItem(plane, 6)->valuedouble;

//         printf("[%d] ID: %s | Flight: %s | Pos: %.4f, %.4f\n", i+1, icao, callsign, lat, lon);
//     }

//     cJSON_Delete(root);
// }

void airplane_fetcher_task(void *pvParameters) {
    while (1) {
        esp_http_client_config_t config = {
            .url = OPENSKY_URL,
            .event_handler = _http_event_handler,
            .crt_bundle_attach = esp_crt_bundle_attach,
        };

        esp_http_client_handle_t client = esp_http_client_init(&config);
        response_len = 0;
        if (response_buffer) free(response_buffer);
        response_buffer = NULL;

        if (esp_http_client_perform(client) == ESP_OK) {
            if (response_buffer) {
                process_new_flights(response_buffer);
                // parse_and_print_flights(response_buffer);
            }
        }
        esp_http_client_cleanup(client);
        vTaskDelay(pdMS_TO_TICKS(30000));
    }
}