// Day 4 of the Let it Glow Box
#include <stdio.h>
#include "pico/stdlib.h"

#define SEGMENT_1 13
#define SEGMENT_2 12
#define SEGMENT_3 11
#define SEGMENT_4 10
#define SEGMENT_5 9
#define BUTTON_PIN_FIRST 2
#define BUTTON_PIN_SECOND 3
#define NUM_SEGS 5

int segments[] = {SEGMENT_1, SEGMENT_2, SEGMENT_3, SEGMENT_4, SEGMENT_5};
#define MAX_VALUE ((1<<NUM_SEGS)-1)
void pico_initialize_segments(void)
{
    for(int i = 0; i< MAX_VALUE; i++)
    {
        gpio_init(segments[i]);
        gpio_set_dir(segments[i], GPIO_OUT);
    }
}
void pico_button_init(void){
    gpio_init(BUTTON_PIN_FIRST);
    gpio_set_dir(BUTTON_PIN_FIRST, GPIO_IN);
    gpio_pull_down(BUTTON_PIN_FIRST);
    
    gpio_init(BUTTON_PIN_SECOND);
    gpio_set_dir(BUTTON_PIN_SECOND, GPIO_IN);
    gpio_pull_down(BUTTON_PIN_SECOND);
}
void toggle_segment(int segment)
{
    gpio_put(segment, !gpio_get(segment));
}
void test_segments(int *segments, int size)
{
    for(int i = 0; i< size; i++)
    {
        toggle_segment(segments[i]);
        sleep_ms(1000);
    }
}
void display_binary(int value)
{
    for(int i = 0; i< NUM_SEGS; i++)
    {
        gpio_put(segments[i], (value >> i) & 1);
    }
}
int main()
{
    stdio_init_all();
    pico_initialize_segments();
    pico_button_init();

    int count = 0;

    while (true) {

        if(gpio_get(BUTTON_PIN_FIRST))
        {
            if(count < MAX_VALUE) count++;
            display_binary(count);
            sleep_ms(250);
        }

        if(gpio_get(BUTTON_PIN_SECOND))
        {
            if(count > 0) count--;
            display_binary(count);
            sleep_ms(250);
        }
    }
}
