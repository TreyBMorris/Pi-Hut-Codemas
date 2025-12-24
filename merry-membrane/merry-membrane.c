#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"

#include "ws2812.pio.h"
#include "blink.pio.h"

#define KEY_ONE 11
#define KEY_TWO 10
#define KEY_THREE 13
#define KEY_FOUR 12

#define LED_STRIP_PIN 2
#define LED_STRIP_COUNT 15
#define LED_IS_RGBW false

void button_init(void)
{
    gpio_init(KEY_ONE);
    gpio_set_dir(KEY_ONE, GPIO_IN);
    gpio_pull_down(KEY_ONE);

    gpio_init(KEY_TWO);
    gpio_set_dir(KEY_TWO, GPIO_IN);
    gpio_pull_down(KEY_TWO);

    gpio_init(KEY_THREE);
    gpio_set_dir(KEY_THREE, GPIO_IN);
    gpio_pull_down(KEY_THREE);

    gpio_init(KEY_FOUR);
    gpio_set_dir(KEY_FOUR, GPIO_IN);
    gpio_pull_down(KEY_FOUR);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)(g) << 16) |
           ((uint32_t)(r) << 8)  |
           (uint32_t)(b);
}

void put_pixel(PIO pio, int sm, uint32_t pixel_grb) {
    pio_sm_put_blocking(pio, sm, pixel_grb << 8u);
}

void show_strip(PIO pio, int sm, uint32_t *pixels) {
    for (int i = 0; i < LED_STRIP_COUNT; i++) {
        put_pixel(pio, sm, pixels[i]);
    }
}


int main()
{
    stdio_init_all();
    button_init();

    
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);

    ws2812_program_init(pio, sm, offset, LED_STRIP_PIN, 800000, LED_IS_RGBW);


    uint32_t pixels[LED_STRIP_COUNT];

    for(int i =0; i< LED_STRIP_COUNT; i++) {
        pixels[i] = urgb_u32(0, 0, 0); 
    }
    show_strip(pio, sm, pixels);

    while (true) {
        if (gpio_get(KEY_ONE)) {
            printf("Button ONE pressed\n");
            for (int i = 0; i < LED_STRIP_COUNT; i++) {
                pixels[i] = urgb_u32(0, 255, 0);
            }
            show_strip(pio, sm, pixels);
            sleep_ms(1000);
            for (int i = 0; i < LED_STRIP_COUNT; i++) {
                pixels[i] = urgb_u32(0, 0, 0);
            }
            show_strip(pio, sm, pixels);
        }
        if (gpio_get(KEY_TWO)) {
            printf("Button TWO pressed\n");
            for (int i = 0; i < LED_STRIP_COUNT; i++) {
                pixels[i] = urgb_u32(255, 0, 0);
            }
            show_strip(pio, sm, pixels);
            sleep_ms(1000);
            for (int i = 0; i < LED_STRIP_COUNT; i++) {
                pixels[i] = urgb_u32(0, 0, 0);
            }
            show_strip(pio, sm, pixels);
        }
        if (gpio_get(KEY_THREE)) {
            printf("Button THREE pressed\n");
            for (int i = 0; i < LED_STRIP_COUNT; i++) {
                pixels[i] = urgb_u32(0, 0, 255);
            }
            show_strip(pio, sm, pixels);
            sleep_ms(1000);
            for (int i = 0; i < LED_STRIP_COUNT; i++) {
                pixels[i] = urgb_u32(0, 0, 0);
            }
            show_strip(pio, sm, pixels);
        }
        if (gpio_get(KEY_FOUR)) {
            printf("Button FOUR pressed\n");
            for (int i = 0; i < LED_STRIP_COUNT; i++) {
                pixels[i] = urgb_u32(255, 255, 255);
            }
            show_strip(pio, sm, pixels);
            sleep_ms(1000);
            for (int i = 0; i < LED_STRIP_COUNT; i++) {
                pixels[i] = urgb_u32(0, 0, 0);
            }
            show_strip(pio, sm, pixels);
        }
    }

}
