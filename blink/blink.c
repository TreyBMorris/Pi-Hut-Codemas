/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/stdlib.h"
#include <stdio.h>

#ifndef LED_DELAY_MS
#define LED_DELAY_MS 500
#endif

#ifndef PICO_DEFAULT_LED_PIN
#warning blink example requires a board with a default LED pin defined
#endif

// Matches the GPIO pins connected to the LEDs on the board
#define LED_PIN_FIRST 18
#define LED_PIN_SECOND 19
#define LED_PIN_THIRD 20 

// Initialize the LED
void pico_led_init(void) {
#ifdef PICO_DEFAULT_LED_PIN
    // A device like Pico that uses a GPIO for the LED will define PICO_DEFAULT_LED_PIN
    // so we can use normal GPIO functionality to turn the led on and off

    // Initialize additional LED pins
    gpio_init(LED_PIN_FIRST);
    gpio_set_dir(LED_PIN_FIRST, GPIO_OUT);
    gpio_init(LED_PIN_SECOND);
    gpio_set_dir(LED_PIN_SECOND, GPIO_OUT);
    gpio_init(LED_PIN_THIRD);
    gpio_set_dir(LED_PIN_THIRD, GPIO_OUT);
#endif
}

// Turn the led on or off
void pico_set_led(bool led_on) {
#if defined(PICO_DEFAULT_LED_PIN)
    // Just set the GPIO on or off
    gpio_put(LED_PIN_FIRST, led_on);
    gpio_put(LED_PIN_SECOND, led_on);
    gpio_put(LED_PIN_THIRD, led_on);
#endif
}

// Toggle a specific LED on or off
void pico_toggle_led(int led, bool led_on) {
    // Just set the GPIO on or off
    switch(led) {
        case 1:
            gpio_put(LED_PIN_FIRST, led_on);
            break;
        case 2:
            gpio_put(LED_PIN_SECOND, led_on);
            break;
        case 3:
            gpio_put(LED_PIN_THIRD, led_on);
            break;
        default:
            break;
    }
}

int main() {
    // Initialize the LEDs
    pico_led_init();
    int counter = 0;

    while (counter < 5) {
        printf("Blinking LED Count %d\n", counter);
        pico_set_led(true);
        sleep_ms(LED_DELAY_MS);
        pico_set_led(false);
        sleep_ms(LED_DELAY_MS);
        counter ++;
    }

    while(true){
        // Sequentially toggle each LED on and off
        for(int i = 1; i <= 3; i++) {
            pico_toggle_led(i, true);
            sleep_ms(LED_DELAY_MS);
            pico_toggle_led(i, false);
            sleep_ms(LED_DELAY_MS);
        }
    }
}
