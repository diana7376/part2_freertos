/*
 * btn_drv.cpp
 * ECAL – Button driver with millis()-based debounce (INPUT_PULLUP, active LOW)
 * Uses millis() directly so it works regardless of FreeRTOS tick rate.
 */

#include "btn_drv.h"
#include "hw_config.h"
#include <Arduino.h>

#define DEBOUNCE_MS  40   // signal must be stable for 40 ms

static uint8_t  debounced         = 1;   // HIGH = not pressed
static uint8_t  prev_deb          = 1;
static uint32_t debounce_start_ms = 0;   // 0 = not timing

static uint8_t flag_fell = 0;
static uint8_t flag_rose = 0;

void btn_drv_init(void) {
    pinMode(PIN_BUTTON, INPUT_PULLUP);
    debounced         = (uint8_t)digitalRead(PIN_BUTTON);
    prev_deb          = debounced;
    debounce_start_ms = 0;
}

void btn_drv_tick(void) {
    flag_fell = 0;
    flag_rose = 0;

    uint8_t raw = (uint8_t)digitalRead(PIN_BUTTON);

    if (raw != debounced) {
        if (debounce_start_ms == 0) {
            debounce_start_ms = millis();
            if (debounce_start_ms == 0)
                debounce_start_ms = 1;   // avoid 0 sentinel
        }
        if ((millis() - debounce_start_ms) >= DEBOUNCE_MS) {
            prev_deb          = debounced;
            debounced         = raw;
            debounce_start_ms = 0;
            if (prev_deb == 1 && debounced == 0) flag_fell = 1;
            if (prev_deb == 0 && debounced == 1) flag_rose = 1;
        }
    } else {
        debounce_start_ms = 0;
    }
}

uint8_t btn_is_pressed(void) { return (debounced == 0) ? 1 : 0; }
uint8_t btn_fell(void)       { return flag_fell; }
uint8_t btn_rose(void)       { return flag_rose; }