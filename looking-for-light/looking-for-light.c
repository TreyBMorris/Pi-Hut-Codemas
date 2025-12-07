#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

#ifndef LED_PIN_FIRST
#define LED_PIN_FIRST 18
#endif
#ifndef LED_PIN_SECOND
#define LED_PIN_SECOND 19
#endif
#ifndef LED_PIN_THIRD
#define LED_PIN_THIRD 20
#endif
#ifndef ADC_PIN
#define ADC_PIN 26
#endif


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


int main()
{
    stdio_init_all();

    adc_init();
    adc_gpio_init(ADC_PIN); 
    adc_select_input(0); 

    pico_led_init();




    while (true) {
        uint16_t result = adc_read(); // Read the ADC value
        float lightpercent = (result / 4095.0f) * 100.0f; // Convert to percentage
        printf("Light Level: %.2f%%\n", lightpercent);
        sleep_ms(1000);

        // Control LEDs based on light percentage
        // This isn't how I would normally do it, but it works for this simple example
        if(lightpercent <= 30.0f) {
            pico_toggle_led(1, true);
            pico_toggle_led(2, false);
            pico_toggle_led(3, false);
        } else if(lightpercent < 60.0f && lightpercent > 30.0f) {
            pico_toggle_led(1, false);
            pico_toggle_led(2, true);
            pico_toggle_led(3, false);
        } else if (lightpercent >= 60.0f) {
            pico_toggle_led(1, false);
            pico_toggle_led(2, false);
            pico_toggle_led(3, true);
        } else {
            pico_toggle_led(1, false);
            pico_toggle_led(2, false);
            pico_toggle_led(3, false);
        }
    }
}
