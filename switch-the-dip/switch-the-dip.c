// Day 5 of the Let It Glow Box
#include <stdio.h>
#include "pico/stdlib.h"

#define SEGMENT_1 13
#define SEGMENT_2 12
#define SEGMENT_3 11
#define SEGMENT_4 10
#define SEGMENT_5 9
#define NUM_SEGS 5
#define DIP_1 2
#define DIP_2 3
#define DIP_3 4
#define DIP_4 5
#define DIP_5 6
#define NUM_DIPS 5

int segments[] = {SEGMENT_1, SEGMENT_2, SEGMENT_3, SEGMENT_4, SEGMENT_5};
int dips[] = {DIP_1, DIP_2, DIP_3, DIP_4, DIP_5};

void pico_initialize_segments(void)
{
    for(int i = 0; i< NUM_DIPS; i++)
    {
        gpio_init(segments[i]);
        gpio_set_dir(segments[i], GPIO_OUT);
    }
}

void pico_initialize_dips(void)
{
    for(int i = 0; i < NUM_DIPS; i++)
    {
        gpio_init(dips[i]);
        gpio_set_dir(dips[i], GPIO_IN);
        gpio_pull_down(dips[i]);
    }
}
void toggle_segment(int segment)
{
    gpio_put(segment, !gpio_get(segment));

}

void toggle_segments_all_off(void)
{
    for(int i =0; i< NUM_SEGS; i++)
    {
        gpio_put(segments[i], false);
    }
}
bool last_dip_state[NUM_DIPS] = {false};

void handle_dips(void)
{
    for (int i = 0; i < NUM_DIPS; i++)
    {
        bool current = gpio_get(dips[i]);
        if (current && !last_dip_state[i])
        {
            toggle_segment(segments[i]);
        }
        last_dip_state[i] = current;
    }
}

int main()
{
    stdio_init_all();
    pico_initialize_segments();
    pico_initialize_dips();
    while (true) {
        handle_dips();
        sleep_ms(200);
    }
}
