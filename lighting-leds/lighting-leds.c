// Day 2 of the Let It Glow Box
#include <stdio.h>
#include "pico/stdlib.h"

#define LED_PIN 14

void pico_led_init(void){
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
}
void pico_set_led(bool led_on){
    gpio_put(LED_PIN, led_on);
}
int main()
{
    stdio_init_all();
    pico_led_init();

    for(int i =0; i<10; i++){

        pico_set_led(true);
        sleep_ms(500);
        pico_set_led(false);
        sleep_ms(500);
        printf("Program finished");
    }
}
