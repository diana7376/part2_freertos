/*
 * task_stats_rtos.cpp
 * Task 2 – Statistics + yellow blink (FreeRTOS version)
 *
 * Period  : 20 ms
 * Offset  : 5 ms
 */

#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include "shared_data.h"
#include "led_drv.h"

#define OFFSET_MS      5
#define BLINKS_SHORT   5
#define BLINKS_LONG    10

void Task_Statistics_RTOS(void *pvParameters) {
    (void)pvParameters;

    vTaskDelay(pdMS_TO_TICKS(OFFSET_MS));

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(20);

    for (;;) {
        // Advance yellow blink SM: 20 steps to compensate for 20 ms period
        for (uint8_t i = 0; i < 20; i++) led_yellow_blink_tick();

        uint32_t duration = 0;
        uint8_t  is_long  = 0;

        if (shared_consume_press_event(&duration, &is_long)) {
            shared_stats_add(duration, is_long);
            if (!led_yellow_blink_busy()) {
                led_yellow_blink_start(is_long ? BLINKS_LONG : BLINKS_SHORT);
            }
        }

        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}