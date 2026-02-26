#ifndef SHARED_DATA_H
#define SHARED_DATA_H

/*
 * shared_data.h  (Part 2 – FreeRTOS version)
 * SRV – Thread-safe shared data with mutex protection.
 * Uses a binary semaphore for press-event signalling and
 * a mutex for statistics protection.
 */

#include <stdint.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

// ─── Initialise sync primitives (call once in main before tasks start) ──────
void shared_init(void);

// ─── Press event (semaphore-based signalling) ────────────────────────────────
void    shared_set_press_event(uint32_t duration_ms, uint8_t is_long);
// Returns 1 if event consumed, 0 if none pending
uint8_t shared_consume_press_event(uint32_t *out_duration, uint8_t *out_is_long);

// ─── Statistics (mutex-protected) ───────────────────────────────────────────
typedef struct {
    uint32_t total_presses;
    uint32_t short_presses;
    uint32_t long_presses;
    uint32_t sum_duration_ms;
} PressStats_t;

void         shared_stats_add(uint32_t duration_ms, uint8_t is_long);
PressStats_t shared_stats_get(void);
void         shared_stats_reset(void);

#endif // SHARED_DATA_H
