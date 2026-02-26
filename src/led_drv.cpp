/*
 * led_drv.cpp
 * ECAL – LED driver implementation (Arduino Mega, digital I/O)
 */

#include "led_drv.h"
#include "hw_config.h"
#include <Arduino.h>

// ─── Non-blocking blink state machine ──────────────────────────────────────
static uint8_t  blink_remain  = 0;
static uint8_t  blink_state   = 0;   // 0=off 1=on
static uint16_t blink_timer   = 0;
#define BLINK_HALF_PERIOD_MS  100

void led_drv_init(void) {
    pinMode(PIN_LED_GREEN,  OUTPUT);
    pinMode(PIN_LED_RED,    OUTPUT);
    pinMode(PIN_LED_YELLOW, OUTPUT);
    digitalWrite(PIN_LED_GREEN,  LOW);
    digitalWrite(PIN_LED_RED,    LOW);
    digitalWrite(PIN_LED_YELLOW, LOW);
}

void led_green_on(void)  { digitalWrite(PIN_LED_GREEN, HIGH); }
void led_green_off(void) { digitalWrite(PIN_LED_GREEN, LOW);  }
void led_red_on(void)    { digitalWrite(PIN_LED_RED,   HIGH); }
void led_red_off(void)   { digitalWrite(PIN_LED_RED,   LOW);  }
void led_yellow_on(void) { digitalWrite(PIN_LED_YELLOW, HIGH); }
void led_yellow_off(void){ digitalWrite(PIN_LED_YELLOW, LOW);  }

void led_yellow_blink_start(uint8_t count) {
    blink_remain = count;
    blink_state  = 1;
    blink_timer  = BLINK_HALF_PERIOD_MS;
    digitalWrite(PIN_LED_YELLOW, HIGH);
}

void led_yellow_blink_tick(void) {
    if (blink_remain == 0) return;
    if (blink_timer > 0) { blink_timer--; return; }

    blink_state ^= 1;
    digitalWrite(PIN_LED_YELLOW, blink_state ? HIGH : LOW);
    blink_timer = BLINK_HALF_PERIOD_MS;

    if (blink_state == 0) {
        blink_remain--;
        if (blink_remain == 0) digitalWrite(PIN_LED_YELLOW, LOW);
    }
}

uint8_t led_yellow_blink_busy(void) { return (blink_remain > 0) ? 1 : 0; }
