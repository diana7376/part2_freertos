/*
 * shared_data.cpp  (Part 2 – FreeRTOS)
 * Thread-safe shared data implementation.
 */

#include "shared_data.h"
#include <string.h>

// ─── Sync primitives ─────────────────────────────────────────────────────────
static SemaphoreHandle_t xEventSem   = NULL;  // binary semaphore for press event
static SemaphoreHandle_t xStatsMutex = NULL;  // mutex for statistics

// ─── Press event storage ─────────────────────────────────────────────────────
static volatile uint32_t event_duration = 0;
static volatile uint8_t  event_is_long  = 0;

// ─── Statistics storage ───────────────────────────────────────────────────────
static PressStats_t stats;

void shared_init(void) {
    xEventSem   = xSemaphoreCreateBinary();
    xStatsMutex = xSemaphoreCreateMutex();
    memset(&stats, 0, sizeof(stats));
}

// ─── Press event ─────────────────────────────────────────────────────────────
void shared_set_press_event(uint32_t duration_ms, uint8_t is_long) {
    event_duration = duration_ms;
    event_is_long  = is_long;
    xSemaphoreGive(xEventSem);   // signal Task2
}

uint8_t shared_consume_press_event(uint32_t *out_duration, uint8_t *out_is_long) {
    if (xSemaphoreTake(xEventSem, 0) == pdTRUE) {
        *out_duration = event_duration;
        *out_is_long  = event_is_long;
        return 1;
    }
    return 0;
}

// ─── Statistics ──────────────────────────────────────────────────────────────
void shared_stats_add(uint32_t duration_ms, uint8_t is_long) {
    if (xSemaphoreTake(xStatsMutex, portMAX_DELAY) == pdTRUE) {
        stats.total_presses++;
        stats.sum_duration_ms += duration_ms;
        if (is_long) stats.long_presses++;
        else         stats.short_presses++;
        xSemaphoreGive(xStatsMutex);
    }
}

PressStats_t shared_stats_get(void) {
    PressStats_t snap;
    if (xSemaphoreTake(xStatsMutex, portMAX_DELAY) == pdTRUE) {
        snap = stats;
        xSemaphoreGive(xStatsMutex);
    }
    return snap;
}

void shared_stats_reset(void) {
    if (xSemaphoreTake(xStatsMutex, portMAX_DELAY) == pdTRUE) {
        memset(&stats, 0, sizeof(stats));
        xSemaphoreGive(xStatsMutex);
    }
}
