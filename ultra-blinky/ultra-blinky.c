#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"

#define LED_PIN 2 
#define LED_COUNT 15
#define IS_RGBW false

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)(r) << 8) |
           ((uint32_t)(g) << 16) |
           (uint32_t)(b);
}

void put_pixel(PIO pio, int sm, uint32_t pixel_grb) {
    pio_sm_put_blocking(pio, sm, pixel_grb << 8u);
}
int main()
{
    stdio_init_all();

    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);

    ws2812_program_init(
        pio,
        sm,
        offset,
        LED_PIN,
        800000,
        IS_RGBW
    );

    uint32_t pixels[LED_COUNT];

    for (int i = 0; i < LED_COUNT; i++) {
        pio_sm_put_blocking(pio, sm, urgb_u32(0, 10, 0) << 8u);
    }

 
     while (true) {
        for (int i = 0; i < LED_COUNT; i++) {
            put_pixel(pio, sm, pixels[i]);
        }
        sleep_ms(1000);

        for (int led = 0; led < LED_COUNT; led++) {

            for (int i = 255; i > 10; i--) {

                pixels[led] = urgb_u32(i, i, i);  

                for (int j = 0; j < LED_COUNT; j++) {
                    put_pixel(pio, sm, pixels[j]);
                }

                sleep_ms(1);
            }

            pixels[led] = urgb_u32(10, 0, 0);

            for (int j = 0; j < LED_COUNT; j++) {
                put_pixel(pio, sm, pixels[j]);
            }
        }
    }
}
