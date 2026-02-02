#ifndef AIRPLANE_FETCHER_H
#define AIRPLANE_FETCHER_H

/**
 * @brief FreeRTOS task that periodically fetches flight data.
 * @param pvParameters Standard FreeRTOS task parameters.
 */
void airplane_fetcher_task(void *pvParameters);

#endif