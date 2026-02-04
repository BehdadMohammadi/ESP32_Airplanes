#include "flight_manager.h"
#include "cJSON.h"
#include "esp_log.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "project_config.h"

static const char *TAG = "FLIGHT_MGR";
#define MAX_TRACKED_PLANES 20
flight_data_t watchlist[MAX_TRACKED_PLANES];


#define HOME_LAT MY_HOME_LAT
#define HOME_LON MY_HOME_LON

#define EARTH_RADIUS_KM 6371.0
#define DEG_TO_RAD(deg) (deg * M_PI / 180.0)


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


double calculate_haversine(double lat1, double lon1, double lat2, double lon2) {
    // Distance between latitudes and longitudes
    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;

    // Convert to radians
    double r_lat1 = lat1 * M_PI / 180.0;
    double r_lat2 = lat2 * M_PI / 180.0;

    // Apply formula
    double a = pow(sin(dLat / 2), 2) +
               pow(sin(dLon / 2), 2) * cos(r_lat1) * cos(r_lat2);
    double c = 2 * asin(sqrt(a));
    
    return EARTH_RADIUS_KM * c;
}

// Comparator for qsort (Sorts ascending: closest planes first)
int compare_flights(const void *a, const void *b) {
    flight_data_t *f1 = (flight_data_t *)a;
    flight_data_t *f2 = (flight_data_t *)b;
    
    // Handle empty slots (move them to the end)
    if (strlen(f1->icao24) == 0) return 1;
    if (strlen(f2->icao24) == 0) return -1;

    if (f1->distance < f2->distance) return -1;
    if (f1->distance > f2->distance) return 1;
    return 0;
}


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
        double dist = calculate_haversine(HOME_LAT, HOME_LON, lat, lon);
        
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
                watchlist[j].distance = dist;
                found = true;
                // ESP_LOGI(TAG, "INBOUND: %s | Alt: %.0fm | Spd: %.0f km/h", call, alt, vel);
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
                    watchlist[j].distance = dist;
                    
                    // ESP_LOGI(TAG, "INBOUND: %s | Alt: %.0fm | Spd: %.0f km/h", call, alt, vel);
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

    // Sort watchlist by distance
    qsort(watchlist, MAX_TRACKED_PLANES, sizeof(flight_data_t), compare_flights);

    ESP_LOGI(TAG, "--- Sorted Flights by Distance ---");
    for (int i = 0; i < MAX_TRACKED_PLANES; i++) {
        if (strlen(watchlist[i].icao24) > 0) 
        {
            ESP_LOGI(TAG, "[%d]- INBOUND: %s | Dist: %.2f km | Lat: %.5f | Lon: %.5f | Alt: %.0fm | Spd: %.0f km/h", i+1, watchlist[i].callsign, watchlist[i].distance, watchlist[i].latitude, watchlist[i].longitude, watchlist[i].altitude, watchlist[i].speed);
            
            // printf("[%d] %s | Dist: %.2f km | Alt: %.0fm | Spd: %.0f km/h\n", 
            //    i+1, watchlist[i].callsign, watchlist[i].distance, 
            //    watchlist[i].altitude, watchlist[i].speed);
        }
    }
}