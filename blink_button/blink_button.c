#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

// Define default pin numbers if not already defined
#ifndef LED_PIN_FIRST 
#define LED_PIN_FIRST 18
#endif
#ifndef LED_PIN_SECOND
#define LED_PIN_SECOND 19
#endif
#ifndef LED_PIN_THIRD
#define LED_PIN_THIRD 20
#endif

// Define default button pin numbers if not already defined
#ifndef BUTTON_PIN_FIRST
#define BUTTON_PIN_FIRST 13
#endif
#ifndef BUTTON_PIN_SECOND
#define BUTTON_PIN_SECOND 8
#endif
#ifndef BUTTON_PIN_THIRD
#define BUTTON_PIN_THIRD 3
#endif

// Initialize LED GPIO pins as outputs
void pico_led_init(void)
{
    gpio_init(LED_PIN_FIRST);
    gpio_set_dir(LED_PIN_FIRST, GPIO_OUT);
    
    gpio_init(LED_PIN_SECOND);
    gpio_set_dir(LED_PIN_SECOND, GPIO_OUT);
    
    gpio_init(LED_PIN_THIRD);
    gpio_set_dir(LED_PIN_THIRD, GPIO_OUT);
}

// Initialize button GPIO pins as inputs
void pico_button_init(void)
{
    gpio_init(BUTTON_PIN_FIRST);
    gpio_set_dir(BUTTON_PIN_FIRST, GPIO_IN);
    gpio_pull_down(BUTTON_PIN_FIRST);  

    gpio_init(BUTTON_PIN_SECOND);
    gpio_set_dir(BUTTON_PIN_SECOND, GPIO_IN);
    gpio_pull_down(BUTTON_PIN_SECOND);

    gpio_init(BUTTON_PIN_THIRD);
    gpio_set_dir(BUTTON_PIN_THIRD, GPIO_IN);
    gpio_pull_down(BUTTON_PIN_THIRD);
}



// Set the state of all LEDs
void pico_set_led(bool led_on)
{
    gpio_put(LED_PIN_FIRST, led_on);
    gpio_put(LED_PIN_SECOND, led_on);
    gpio_put(LED_PIN_THIRD, led_on);
}

// Toggle the state of a specific LED pin
void pico_toggle_led_pin(int led_pin)
{
    gpio_put(led_pin, !gpio_get(led_pin));
}



int main()
{
    stdio_init_all();

    // Initialize LEDs and buttons
    pico_led_init();
    pico_button_init();

    // Initial previous button states
    bool prev1 = gpio_get(BUTTON_PIN_FIRST);
    bool prev2 = gpio_get(BUTTON_PIN_SECOND);
    bool prev3 = gpio_get(BUTTON_PIN_THIRD);
    
    while(true)
    {
        // Read current button states
        bool b1 = gpio_get(BUTTON_PIN_FIRST);
        bool b2 = gpio_get(BUTTON_PIN_SECOND);
        bool b3 = gpio_get(BUTTON_PIN_THIRD);

        // Check for button presses and toggle corresponding LEDs
        if(b1 && !prev1)
        {
            pico_toggle_led_pin(LED_PIN_FIRST);
            sleep_ms(200);
        }
        prev1 = b1;

        if(b2 && !prev2)
        {
            pico_toggle_led_pin(LED_PIN_SECOND);
            sleep_ms(200);
        }
        prev2 = b2;

        if(b3 && !prev3)
        {
            pico_toggle_led_pin(LED_PIN_THIRD);
            sleep_ms(200);
        }

        prev3 = b3;

        // Small delay to debounce and reduce CPU usage
        sleep_ms(10);
    }
    


}
