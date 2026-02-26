#ifndef BTN_DRV_H
#define BTN_DRV_H

/*
 * btn_drv.h
 * ECAL – Button driver (debounced, non-blocking)
 */

#include <stdint.h>

void     btn_drv_init(void);
void     btn_drv_tick(void);          /* call every 1 ms */
uint8_t  btn_is_pressed(void);        /* 1 while physically held */
uint8_t  btn_fell(void);              /* 1 for one tick on press  */
uint8_t  btn_rose(void);              /* 1 for one tick on release */

#endif // BTN_DRV_H
