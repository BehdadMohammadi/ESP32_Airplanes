#ifndef WIFI_CONNECTOR_H
#define WIFI_CONNECTOR_H

#include "esp_err.h"

/**
 * @brief Initializes Wi-Fi in Station mode and blocks until connected.
 * * @return esp_err_t ESP_OK if connected, ESP_FAIL otherwise.
 */
esp_err_t wifi_init_sta(void);

#endif