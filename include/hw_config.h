#ifndef HW_CONFIG_H
#define HW_CONFIG_H

/*
 * hw_config.h
 * Hardware pin definitions for Arduino Mega
 * Circuit: Button on D3, Yellow LED on D9, Red LED on D11, Green LED on D12
 */

// ─── Pin definitions ───────────────────────────────────────────────────────
#define PIN_BUTTON      3   // Push button (INPUT_PULLUP)
#define PIN_LED_GREEN   12  // Green  LED  – short press indicator
#define PIN_LED_RED     11  // Red    LED  – long  press indicator
#define PIN_LED_YELLOW  9   // Yellow LED  – blink counter feedback

// ─── Press duration threshold ──────────────────────────────────────────────
#define PRESS_SHORT_THRESHOLD_MS  500   // < 500 ms → short press

// ─── Scheduler tick period ─────────────────────────────────────────────────
#define TICK_PERIOD_MS  1   // Timer1 fires every 1 ms

#endif // HW_CONFIG_H
