#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

#ifndef PIR_PIN
#define PIR_PIN 26
#endif
#ifndef BUZZER_PIN
#define BUZZER_PIN 13
#endif

#ifndef LED_PIN_FIRST
#define LED_PIN_FIRST 18
#endif
#ifndef LED_PIN_SECOND
#define LED_PIN_SECOND 19
#endif
#ifndef LED_PIN_THIRD
#define LED_PIN_THIRD 20
#endif

typedef struct
{
    uint pin;
    uint slice;
    uint channel;
} Buzzer;

// Initialize buzzer
Buzzer buzzer_init(uint pin)
{
    gpio_set_function(pin, GPIO_FUNC_PWM);

    Buzzer b;
    b.pin = pin;
    b.slice = pwm_gpio_to_slice_num(pin);
    b.channel = pwm_gpio_to_channel(pin);

    pwm_set_wrap(b.slice, 65535);

    pwm_set_enabled(b.slice, true);

    return b;
}

void pico_led_init(void)
{
    gpio_init(LED_PIN_FIRST);
    gpio_set_dir(LED_PIN_FIRST, GPIO_OUT);
    gpio_init(LED_PIN_SECOND);
    gpio_set_dir(LED_PIN_SECOND, GPIO_OUT);
    gpio_init(LED_PIN_THIRD);
    gpio_set_dir(LED_PIN_THIRD, GPIO_OUT);
}

void toggle_pico_leds(void)
{
    gpio_put(LED_PIN_FIRST, !gpio_get(LED_PIN_FIRST));
    gpio_put(LED_PIN_SECOND, !gpio_get(LED_PIN_SECOND));
    gpio_put(LED_PIN_THIRD, !gpio_get

    (LED_PIN_THIRD));
}

void buzzer_set_duty(Buzzer b, uint16_t duty)
{
    pwm_set_chan_level(b.slice, b.channel, duty);
}

void buzzer_set_freq(Buzzer b, float freq_hz) {
    float clock = 125000000.0f;

    float divider = clock / (freq_hz * 65536.0f);

    pwm_set_clkdiv(b.slice, divider);
}


void buzzer_off(Buzzer b)
{
    pwm_set_chan_level(b.slice, b.channel, 0);
}

void alarm_sound(Buzzer b)
{
    int count = 5;

    for (int i = 0; i < count; i++)
    {
        buzzer_set_duty(b, 32768); 
        buzzer_set_freq(b, 5000.0f);
        toggle_pico_leds();

        sleep_ms(1000);

        buzzer_set_freq(b, 500.0f);
        toggle_pico_leds();
        sleep_ms(1000);
    }

    buzzer_off(b);
}

int main()
{
    stdio_init_all();
    pico_led_init();
    Buzzer buzzer = buzzer_init(BUZZER_PIN);

    gpio_init(PIR_PIN);
    gpio_set_dir(PIR_PIN, GPIO_IN);
    gpio_pull_down(PIR_PIN);

    printf("Waiting for PIR Motion Sensor...\n");
    sleep_ms(10000);
    printf("PIR Motion Sensor Monitoring Started\n");
    while (1)
    {
        if (gpio_get(PIR_PIN))
        {
            printf("Motion Detected!\n");
            alarm_sound(buzzer);
            printf("Monitoring Resumed...\n");
        }
    }
}
