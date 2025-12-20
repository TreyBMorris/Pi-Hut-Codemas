#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/adc.h"
#include "ws2812.pio.h"

#define ADC_PIN 28
#define WS2812_PIN 2
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

    adc_init();

    adc_gpio_init(ADC_PIN);
    adc_select_input(2);

    PIO pio = pio0;
    uint sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);

    ws2812_program_init(
        pio, sm, offset, WS2812_PIN, 800000, IS_RGBW
    );

    while (true)
    {
       uint16_t raw = adc_read();

        float flash = raw / 4095.0f;

        uint8_t g = rand() % 256;
        uint8_t r = rand() % 256;
        uint8_t b = rand() % 256;

        put_pixel(pio, sm, urgb_u32(r, g, b));

        sleep_ms((uint32_t)(flash * 1000)); 
    }
    

}
