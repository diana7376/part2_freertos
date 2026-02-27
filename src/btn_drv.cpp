/*
 * btn_drv.cpp
 * ECAL – Button driver with debounce (INPUT_PULLUP, active LOW)
 */

#include "btn_drv.h"
#include "hw_config.h"
#include <Arduino.h>

#define DEBOUNCE_MS  2  // 5 consecutive same reads @ 4 ms = 20 ms

static uint8_t raw_prev    = 1;  // last raw reading
static uint8_t debounced   = 1;  // stable debounced state (HIGH = not pressed)
static uint8_t prev_deb    = 1;
static uint16_t deb_timer  = 0;

static uint8_t flag_fell   = 0;
static uint8_t flag_rose   = 0;

void btn_drv_init(void) {
    pinMode(PIN_BUTTON, INPUT_PULLUP);
    raw_prev  = digitalRead(PIN_BUTTON);
    debounced = raw_prev;
    prev_deb  = raw_prev;
}

void btn_drv_tick(void) {
    flag_fell = 0;
    flag_rose = 0;

    uint8_t raw = (uint8_t)digitalRead(PIN_BUTTON);

    if (raw != debounced) {
        deb_timer++;
        if (deb_timer >= DEBOUNCE_MS) {
            prev_deb  = debounced;
            debounced = raw;
            deb_timer = 0;
            if (prev_deb == 1 && debounced == 0) flag_fell = 1;  // press
            if (prev_deb == 0 && debounced == 1) flag_rose = 1;  // release
        }
    } else {
        deb_timer = 0;
    }
    raw_prev = raw;
}

uint8_t btn_is_pressed(void) { return (debounced == 0) ? 1 : 0; }
uint8_t btn_fell(void)       { return flag_fell; }
uint8_t btn_rose(void)       { return flag_rose; }
