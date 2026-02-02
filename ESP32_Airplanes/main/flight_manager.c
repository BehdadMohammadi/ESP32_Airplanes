#include "flight_manager.h"
#include "cJSON.h"
#include "esp_log.h"
#include <string.h>

static const char *TAG = "FLIGHT_MGR";
#define MAX_TRACKED_PLANES 20
flight_data_t watchlist[MAX_TRACKED_PLANES];

void flight_manager_init(void) {
    memset(watchlist, 0, sizeof(watchlist));
}

void process_new_flights(const char *json_str) {
    cJSON *root = cJSON_Parse(json_str);
    if (!root) return;

    cJSON *states = cJSON_GetObjectItem(root, "states");
    if (!cJSON_IsArray(states)) {
        cJSON_Delete(root);
        return;
    }

    // Mark existing for activity tracking
    for (int i = 0; i < MAX_TRACKED_PLANES; i++) watchlist[i].is_active = false;

    int current_api_count = cJSON_GetArraySize(states);
    for (int i = 0; i < current_api_count; i++) {
        cJSON *plane = cJSON_GetArrayItem(states, i);

        // Filter: Ground planes (Index 8)
        cJSON *ground_item = cJSON_GetArrayItem(plane, 8);
        if (cJSON_IsTrue(ground_item)) continue;

        // Extract Data
        char *icao = cJSON_GetArrayItem(plane, 0)->valuestring;
        char *call = cJSON_GetArrayItem(plane, 1)->valuestring;
        double lon = cJSON_GetArrayItem(plane, 5)->valuedouble;
        double lat = cJSON_GetArrayItem(plane, 6)->valuedouble;
        
        // Altitude (Index 7) & Velocity (Index 9)
        // Note: These can be NULL in the API if no signal received
        cJSON *alt_item = cJSON_GetArrayItem(plane, 7);
        cJSON *vel_item = cJSON_GetArrayItem(plane, 9);
        float alt = cJSON_IsNumber(alt_item) ? alt_item->valuedouble : 0;
        float vel = cJSON_IsNumber(vel_item) ? vel_item->valuedouble * 3.6 : 0; // m/s to km/h

        // Update or Add
        bool found = false;
        for (int j = 0; j < MAX_TRACKED_PLANES; j++) {
            if (strcmp(watchlist[j].icao24, icao) == 0) {
                watchlist[j].latitude = lat;
                watchlist[j].longitude = lon;
                watchlist[j].altitude = alt;
                watchlist[j].speed = vel;
                watchlist[j].is_active = true;
                found = true;
                ESP_LOGI(TAG, "INBOUND: %s | Alt: %.0fm | Spd: %.0f km/h", call, alt, vel);
                break;
            }
        }

        if (!found) {
            for (int j = 0; j < MAX_TRACKED_PLANES; j++) {
                if (strlen(watchlist[j].icao24) == 0) {
                    strncpy(watchlist[j].icao24, icao, 8);
                    strncpy(watchlist[j].callsign, call, 8);
                    watchlist[j].latitude = lat;
                    watchlist[j].longitude = lon;
                    watchlist[j].altitude = alt;
                    watchlist[j].speed = vel;
                    watchlist[j].is_active = true;
                    
                    ESP_LOGI(TAG, "INBOUND: %s | Alt: %.0fm | Spd: %.0f km/h", call, alt, vel);
                    break;
                }
            }
        }
    }

    // Remove planes that disappeared (out of range or landed)
    for (int i = 0; i < MAX_TRACKED_PLANES; i++) {
        if (strlen(watchlist[i].icao24) > 0 && !watchlist[i].is_active) {
            ESP_LOGI(TAG, "EXITED: %s", watchlist[i].callsign);
            memset(&watchlist[i], 0, sizeof(flight_data_t));
        }
    }

    cJSON_Delete(root);
}