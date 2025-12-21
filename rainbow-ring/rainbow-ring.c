#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"

#define WS2812_PIN 2
#define LED_COUNT  12
#define IS_RGBW    false

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
    uint sm = 0;

    uint offset = pio_add_program(pio, &ws2812_program);

    ws2812_program_init(
        pio,
        sm,
        offset,
        WS2812_PIN,
        800000,
        IS_RGBW
    );

    sleep_ms(100);

    while (true)
    {
        for (int i = 0; i < LED_COUNT; i++)
        {
            put_pixel(pio, sm, urgb_u32(50, 0, 0));
        }

        sleep_ms(1000);
    }
}
