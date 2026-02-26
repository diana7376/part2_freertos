/*
 * task_button_rtos.cpp
 * Task 1 – Button measurement (FreeRTOS version)
 *
 * Period  : 20 ms  (vTaskDelayUntil)
 * Offset  : 0 ms
 *
 * Same logic as bare-metal version but uses vTaskDelayUntil for
 * precise periodic execution. LED auto-off timer uses a simple
 * counter decremented 20 ms per iteration.
 */

#include <Arduino_FreeRTOS.h>
#include "btn_drv.h"
#include "led_drv.h"
#include "shared_data.h"
#include "hw_config.h"
#include <Arduino.h>

#define TASK1_PERIOD_MS    20
#define LED_ON_DURATION_MS 300
#define OFFSET_MS          0

void Task_ButtonMeasure_RTOS(void *pvParameters) {
    (void)pvParameters;

    // Apply offset
    vTaskDelay(pdMS_TO_TICKS(OFFSET_MS));

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(TASK1_PERIOD_MS);

    uint32_t press_start_ms = 0;
    uint32_t led_off_timer  = 0;
    uint8_t  led_is_long    = 0;

    for (;;) {
        btn_drv_tick();

        // LED auto-off
        if (led_off_timer > 0) {
            if (led_off_timer <= TASK1_PERIOD_MS) {
                led_off_timer = 0;
                if (led_is_long) led_red_off();
                else             led_green_off();
            } else {
                led_off_timer -= TASK1_PERIOD_MS;
            }
        }

        if (btn_fell()) {
            press_start_ms = millis();
        }

        if (btn_rose()) {
            uint32_t duration = millis() - press_start_ms;
            uint8_t  is_long  = (duration >= PRESS_SHORT_THRESHOLD_MS) ? 1 : 0;

            if (is_long) { led_red_on();   led_is_long = 1; }
            else         { led_green_on(); led_is_long = 0; }
            led_off_timer = LED_ON_DURATION_MS;

            shared_set_press_event(duration, is_long);
        }

        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}
