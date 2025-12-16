// Day 3 of the Let It Glow Box
#include <stdio.h>
#include "pico/stdlib.h"


#define LED_PIN 14
#define BUTTON_PIN_FIRST 2
#define BUTTON_PIN_SECOND 3

void pico_led_init(void){
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
}
void pico_button_init(void){
    gpio_init(BUTTON_PIN_FIRST);
    gpio_set_dir(BUTTON_PIN_FIRST, GPIO_IN);
    gpio_pull_down(BUTTON_PIN_FIRST);
    
    gpio_init(BUTTON_PIN_SECOND);
    gpio_set_dir(BUTTON_PIN_SECOND, GPIO_IN);
    gpio_pull_down(BUTTON_PIN_SECOND);
}
void pico_set_led(bool led_on){
    gpio_put(LED_PIN, led_on);
}
int main()
{
    stdio_init_all();

    pico_button_init();
    pico_led_init();

    pico_set_led(true);
    sleep_ms(2000);
    printf("Initializing program...\n");
    sleep_ms(2000);
    pico_set_led(false);
    bool prev1 = gpio_get(BUTTON_PIN_FIRST);
    bool prev2 = gpio_get(BUTTON_PIN_SECOND);

    while(true){
        bool b1 = gpio_get(BUTTON_PIN_FIRST);
        bool b2 = gpio_get(BUTTON_PIN_SECOND);

        if(b1 && !prev1){
            pico_set_led(true);
            sleep_ms(200);
        }

        prev1 = b1;

        if(b2 && !prev2){
            pico_set_led(false);
            sleep_ms(200);
        }
        prev2 = b2;


        sleep_ms(10);
    }
}
