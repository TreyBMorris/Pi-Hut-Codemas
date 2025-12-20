#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"

#define LED1_PIN 2
#define LED2_PIN 5
#define LED_COUNT 1
#define IS_RGBW false

static inline void put_pixel(PIO pio, uint sm, uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio, sm, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)g << 16) |
           ((uint32_t)r << 8)  |
           (uint32_t)b;
}
int main()
{
    stdio_init_all();

    PIO pio = pio0;
    uint sm1 = 0;
    uint sm2 = 1;

    uint offset = pio_add_program(pio, &ws2812_program);

    ws2812_program_init(
        pio, sm1, offset, LED1_PIN, 800000, IS_RGBW
    );

    ws2812_program_init(
        pio, sm2, offset, LED2_PIN, 800000, IS_RGBW
    );

    while (true)
    {
        for (int i = 0; i < 255; i++)
        {
            put_pixel(pio, sm1, urgb_u32(i, 0, 0));
            sleep_ms(5);
        }

        put_pixel(pio, sm1, urgb_u32(0, 0, 0));

        for (int i = 254; i >= 0; i--)
        {
            put_pixel(pio, sm2, urgb_u32(i, 0, 0));
            sleep_ms(5);
        }
    }
}
