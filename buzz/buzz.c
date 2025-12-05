#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

// Define the buzzer pin
#ifndef BUZZER_PIN
#define BUZZER_PIN 13
#endif

// Buzzer structure
typedef struct {
    uint pin;
    uint slice;
    uint channel;
} Buzzer;

// Note frequencies
#define NOTE_A  440
#define NOTE_AS 466
#define NOTE_B  494
#define NOTE_C  523
#define NOTE_CS 554
#define NOTE_D  587
#define NOTE_DS 622
#define NOTE_E  659
#define NOTE_F  698
#define NOTE_FS 740
#define NOTE_G  784
#define NOTE_GS 830


// Initialize buzzer
Buzzer buzzer_init(uint pin) {
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
void buzzer_set_freq(Buzzer b, float freq_hz) {
    float clock = 125000000.0f;

    float divider = clock / (freq_hz * 65536.0f);

    pwm_set_clkdiv(b.slice, divider);
}

// Set buzzer duty cycle
void buzzer_set_duty(Buzzer b, uint16_t duty) {
    pwm_set_chan_level(b.slice, b.channel, duty);
}

// Play a tone
void play_tone(Buzzer b, float note, float volume, uint16_t duration_first, uint16_t duration_second) {
    buzzer_set_duty(b, volume);
    buzzer_set_freq(b, note);
    sleep_ms(duration_first);
    buzzer_set_duty(b, 0);
    sleep_ms(duration_second);
}

// Turn buzzer off
void buzzer_off(Buzzer b) {
    pwm_set_chan_level(b.slice, b.channel, 0);
}

// Play Jingle Bells
void play_jingle_bells(){
    Buzzer buzzer = buzzer_init(BUZZER_PIN);

    uint16_t volume = 32768;

    play_tone(buzzer, NOTE_E, volume, 100, 200);
    play_tone(buzzer, NOTE_E, volume, 100, 200);
    play_tone(buzzer, NOTE_E, volume, 100, 500);


    play_tone(buzzer, NOTE_E, volume, 100, 200);
    play_tone(buzzer, NOTE_E, volume, 100, 200);
    play_tone(buzzer, NOTE_E, volume, 100, 500);


    play_tone(buzzer, NOTE_E, volume, 100, 200);
    play_tone(buzzer, NOTE_G, volume, 100, 200);
    play_tone(buzzer, NOTE_C, volume, 100, 200);
    play_tone(buzzer, NOTE_D, volume, 100, 200);
    play_tone(buzzer, NOTE_E, volume, 100, 200);

    buzzer_off(buzzer);
}



int main() {
    stdio_init_all();

    play_jingle_bells();

    while(true){}
}
