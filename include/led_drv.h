#ifndef LED_DRV_H
#define LED_DRV_H

/*
 * led_drv.h
 * ECAL – LED driver interface (green, red, yellow)
 */

#include <stdint.h>

void led_drv_init(void);

void led_green_on(void);
void led_green_off(void);

void led_red_on(void);
void led_red_off(void);

void led_yellow_on(void);
void led_yellow_off(void);

/* Non-blocking blink state machine – call from Task2 each tick */
void led_yellow_blink_start(uint8_t count);   /* start N blinks */
void led_yellow_blink_tick(void);             /* advance state machine */
uint8_t led_yellow_blink_busy(void);          /* 1 while blinking */

#endif // LED_DRV_H
