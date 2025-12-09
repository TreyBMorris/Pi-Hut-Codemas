#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

#define TILT_PIN 26
#define BUZZER_PIN 13
// Buzzer structure
typedef struct
{
    uint pin;
    uint slice;
    uint channel;
} Buzzer;
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

// Set buzzer frequency
void buzzer_set_freq(Buzzer b, float freq_hz)
{
    float clock = 125000000.0f;

    float divider = clock / (freq_hz * 65536.0f);

    pwm_set_clkdiv(b.slice, divider);
}

// Set buzzer duty cycle
void buzzer_set_duty(Buzzer b, uint16_t duty)
{
    pwm_set_chan_level(b.slice, b.channel, duty);
}

// Turn the buzzer off.
void buzzer_off(Buzzer b)
{
    pwm_set_chan_level(b.slice, b.channel, 0);
}

void tilt_alarm(Buzzer b)
{
    buzzer_set_duty(b, 32768);
    buzzer_set_freq(b, 1000.0f);
    sleep_ms(200);
    buzzer_off(b);
}

int main()
{
    stdio_init_all();

    gpio_init(TILT_PIN);
    gpio_set_dir(TILT_PIN, GPIO_IN);
    gpio_pull_down(TILT_PIN);

    Buzzer buzzer = buzzer_init(BUZZER_PIN);

    int tiltCounter = 0;
    int state = 0;

    while (true)
    {
        if (state == 0 && gpio_get(TILT_PIN) == 1)
        {
            printf("Tilt detected!\n");
            tiltCounter++;
            state = 1;
            tilt_alarm(buzzer);
            printf("Tilt Count: %d\n", tiltCounter);
        }
        if (state == 1 && gpio_get(TILT_PIN) == 0)
        {
            state = 0;
        }
        sleep_ms(500);
    }
}
