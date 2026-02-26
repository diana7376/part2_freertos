/*
 * task_report_rtos.cpp
 * Task 3 – Periodic STDIO report (FreeRTOS version)
 *
 * Period  : 10 000 ms
 * Offset  : 10 ms
 */

#include <Arduino_FreeRTOS.h>
#include "shared_data.h"
#include <stdio.h>

#define REPORT_PERIOD_MS  10000
#define OFFSET_MS         10

void Task_Report_RTOS(void *pvParameters) {
    (void)pvParameters;

    vTaskDelay(pdMS_TO_TICKS(OFFSET_MS));

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(REPORT_PERIOD_MS);

    for (;;) {
        PressStats_t s   = shared_stats_get();
        uint32_t avg_ms  = (s.total_presses > 0)
                           ? (s.sum_duration_ms / s.total_presses) : 0;

        printf("\n========== PRESS REPORT ==========\n");
        printf("Total presses   : %lu\n",  (unsigned long)s.total_presses);
        printf("Short presses   : %lu\n",  (unsigned long)s.short_presses);
        printf("Long  presses   : %lu\n",  (unsigned long)s.long_presses);
        printf("Average duration: %lu ms\n", (unsigned long)avg_ms);
        printf("==================================\n\n");

        shared_stats_reset();

        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}
