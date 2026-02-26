/*
 * ============================================================
 *  LAB 3 – PART 2: FreeRTOS Preemptive Multitasking
 *  Platform : Arduino Mega (ATmega2560) + Arduino_FreeRTOS
 *
 *  Circuit (same as Part 1):
 *    D3  – Push button (INPUT_PULLUP)
 *    D9  – Yellow LED + 220Ω → GND
 *    D11 – Red    LED + 220Ω → GND
 *    D12 – Green  LED + 220Ω → GND
 *
 *  Install library: "FreeRTOS" by Richard Barry (Arduino Library Manager)
 *  or add to platformio.ini:  lib_deps = feilipu/FreeRTOS@^10.4.6-8
 *
 *  Synchronisation:
 *    - Binary semaphore : Task1 → Task2 (press event signalling)
 *    - Mutex            : protects shared PressStats_t struct
 *
 *  All three tasks have the same priority (tskIDLE_PRIORITY + 1)
 *  to prevent undesired preemption between sequential-style tasks.
 * ============================================================
 */

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <stdio.h>

#include "hw_config.h"
#include "led_drv.h"
#include "btn_drv.h"
#include "shared_data.h"

// ─── Task prototypes ─────────────────────────────────────────────────────────
void Task_ButtonMeasure_RTOS(void *pvParameters);
void Task_Statistics_RTOS(void *pvParameters);
void Task_Report_RTOS(void *pvParameters);

// ─── STDIO → Serial ──────────────────────────────────────────────────────────
static int uart_putchar(char c, FILE *stream) {
    (void)stream;
    Serial.write(c);
    return 0;
}
static FILE uart_stdout;

// ─────────────────────────────────────────────────────────────────────────────
void setup(void) {
    Serial.begin(115200);

    fdev_setup_stream(&uart_stdout, uart_putchar, NULL, _FDEV_SETUP_WRITE);
    stdout = &uart_stdout;

    led_drv_init();
    btn_drv_init();
    shared_init();   // create semaphore + mutex

    // Create tasks – equal priority to behave sequentially within each period
    xTaskCreate(Task_ButtonMeasure_RTOS, "BtnMeasure",  256, NULL,
                tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(Task_Statistics_RTOS,   "Stats",        256, NULL,
                tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(Task_Report_RTOS,       "Report",       512, NULL,
                tskIDLE_PRIORITY + 1, NULL);

    printf("=== Lab3 Part2: FreeRTOS Preemptive Scheduler ===\n");
    printf("Button D%d | Green D%d | Red D%d | Yellow D%d\n",
           PIN_BUTTON, PIN_LED_GREEN, PIN_LED_RED, PIN_LED_YELLOW);
    printf("Report every 10 s.  Press the button!\n\n");

    vTaskStartScheduler();   // hand control to FreeRTOS

    // Should never reach here
    for (;;);
}

void loop(void) {
    // Empty – FreeRTOS scheduler runs
}
