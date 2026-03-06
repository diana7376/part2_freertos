/*
 * task_button_rtos.cpp
 * Task 1 – Button measurement (FreeRTOS version)
 *
 * Runs every 1 FreeRTOS tick (~15ms) to ensure btn_drv_tick() is called
 * frequently enough to catch short presses and debounce via millis().
 */

#include <Arduino_FreeRTOS.h>
#include "btn_drv.h"
#include "led_drv.h"
#include "shared_data.h"
#include "hw_config.h"
#include <Arduino.h>

#define LED_ON_DURATION_MS  300UL

void Task_ButtonMeasure_RTOS(void *pvParameters) {
    (void)pvParameters;

    uint32_t press_start_ms  = millis();
    uint32_t led_off_time_ms = 0;
    uint8_t  led_is_long     = 0;
    uint8_t  press_active    = 0;

    for (;;) {
        btn_drv_tick();

        // ── LED auto-off using millis() ───────────────────────────────────
        if (led_off_time_ms > 0 && millis() >= led_off_time_ms) {
            led_off_time_ms = 0;
            if (led_is_long) led_red_off();
            else             led_green_off();
        }

        // ── Edge detection ────────────────────────────────────────────────
        if (btn_fell()) {
            press_start_ms = millis();
            press_active   = 1;
        }

        if (btn_rose() && press_active) {
            press_active = 0;
            uint32_t duration = millis() - press_start_ms;
            uint8_t  is_long  = (duration >= PRESS_SHORT_THRESHOLD_MS) ? 1 : 0;

            if (is_long) { led_red_on();   led_is_long = 1; }
            else         { led_green_on(); led_is_long = 0; }

            // Schedule LED off time
            led_off_time_ms = millis() + LED_ON_DURATION_MS;

            shared_set_press_event(duration, is_long);
        }

        vTaskDelay(1);   // yield for 1 tick, then re-check button
    }
}