#ifndef FLIGHT_MANAGER_H
#define FLIGHT_MANAGER_H

#include <stdbool.h>

typedef struct {
    char icao24[9];
    char callsign[9];
    double latitude;
    double longitude;
    float altitude; // in meters
    float speed;    // in km/h
    bool is_active;
} flight_data_t;

void flight_manager_init(void);
void process_new_flights(const char *json_str);

#endif