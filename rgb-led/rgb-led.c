#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"


#define LED_PIN 28
#define LED_COUNT 15

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)(g) << 16) |
           ((uint32_t)(r) << 8)  |
           (uint32_t)(b);
}
void ws2812_init(PIO pio, uint sm, uint pin){
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, LED_PIN, 800000, false);
}

// The lights are actually in GRB not RGB format. 
typedef struct{
    uint8_t g;
    uint8_t r;
    uint8_t b;
} Color;

Color colors[] = {
    {0,255,0}, // Red
    {255,0,0}, // Green
    {0,0,255}, // Blue
    {255,255,0}, // Yellow
    {20,255,147} // Pink
};
const int num_colors = sizeof(colors) / sizeof(colors[0]);

int main()
{
    stdio_init_all();


    PIO pio = pio0;
    int sm = 0;
    ws2812_init(pio, sm, LED_PIN);

    Color led_strip[LED_COUNT];

    for(int i =0; i< LED_COUNT; i++){
        led_strip[i] = colors[i% num_colors];
    }


    // Little wave program I wrote 
    while (true) {
        for (int i = 0; i < LED_COUNT; i++) {
            Color c = led_strip[i];
            uint32_t color24 = ((uint32_t)c.g << 16) | ((uint32_t)c.r << 8) | c.b;
            pio_sm_put_blocking(pio, sm, color24 << 8u);
        }

        Color last = led_strip[LED_COUNT - 1];
        for (int i = LED_COUNT - 1; i > 0; i--) {
            led_strip[i] = led_strip[i - 1];
        }
        led_strip[0] = last;

        sleep_ms(100); 
    } 
    
    return 0;
}
