#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

#define LED_RED     18
#define LED_AMBER   19
#define LED_GREEN   20

#define BEAM_PIN    26
#define BUZZER_PIN  13

#define TARGET_SCORE 100
#define GAME_TIME_SECONDS 30

void buzzer_init(uint pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);

    uint slice = pwm_gpio_to_slice_num(pin);
    pwm_config config = pwm_get_default_config();

    pwm_config_set_clkdiv(&config, 125.0f);
    pwm_config_set_wrap(&config, 1000);

    pwm_init(slice, &config, true);
    pwm_set_gpio_level(pin, 0); 
}

void buzzer_on(uint pin, uint level) {
    pwm_set_gpio_level(pin, level);
}

void buzzer_off(uint pin) {
    pwm_set_gpio_level(pin, 0);
}

int main() {
    stdio_init_all();

    gpio_init(LED_RED);
    gpio_init(LED_AMBER);
    gpio_init(LED_GREEN);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_set_dir(LED_AMBER, GPIO_OUT);
    gpio_set_dir(LED_GREEN, GPIO_OUT);

    gpio_init(BEAM_PIN);
    gpio_set_dir(BEAM_PIN, GPIO_IN);
    gpio_pull_down(BEAM_PIN);

    buzzer_init(BUZZER_PIN);

    int scorecounter = 0;
    int state = 0;

    absolute_time_t start_time;

    printf("Game starts after the beep!\n");

    // Long start beep
    buzzer_on(BUZZER_PIN, 500);
    sleep_ms(2000);
    buzzer_off(BUZZER_PIN);

    printf("GO!\n");
    printf("-------------------------------\n");

    start_time = get_absolute_time();

    while (true) {
        sleep_us(100);  
        int elapsed = absolute_time_diff_us(start_time, get_absolute_time()) / 1000000;

        if (elapsed >= GAME_TIME_SECONDS) {
            gpio_put(LED_RED, 0);
            gpio_put(LED_AMBER, 0);
            gpio_put(LED_GREEN, 0);

            buzzer_on(BUZZER_PIN, 500);
            sleep_ms(200);
            buzzer_off(BUZZER_PIN);

            printf("-------------------------------\n");
            printf("GAME OVER! YOU LOSE\n");
            printf("The target was %d, you scored %d\n", TARGET_SCORE, scorecounter);
            printf("-------------------------------\n");

            while (1) tight_loop_contents();
        }

        if (scorecounter >= TARGET_SCORE) {
            gpio_put(LED_RED, 0);
            gpio_put(LED_AMBER, 0);
            gpio_put(LED_GREEN, 0);

            buzzer_on(BUZZER_PIN, 500);
            sleep_ms(200);
            buzzer_off(BUZZER_PIN);

            printf("-------------------------------\n");
            printf("YOU WIN!\n");
            printf("You took %d seconds!\n", elapsed);
            printf("-------------------------------\n");

            while (1) tight_loop_contents();
        }

        if (state == 0 && gpio_get(BEAM_PIN) == 0) {
            scorecounter++;
            state = 1;

            printf("SCORE = %d / %d\n", scorecounter, TARGET_SCORE);
            printf("Time remaining: %d\n", GAME_TIME_SECONDS - elapsed);

            if (scorecounter < (TARGET_SCORE * 33 / 100)) {
                gpio_put(LED_RED, 1);
                gpio_put(LED_AMBER, 0);
                gpio_put(LED_GREEN, 0);
            }
            else if (scorecounter < (TARGET_SCORE * 66 / 100)) {
                gpio_put(LED_RED, 1);
                gpio_put(LED_AMBER, 1);
                gpio_put(LED_GREEN, 0);
            }
            else {
                gpio_put(LED_RED, 1);
                gpio_put(LED_AMBER, 1);
                gpio_put(LED_GREEN, 1);
            }
        }

        if (state == 1 && gpio_get(BEAM_PIN) == 1) {
            state = 0;
        }
    }
}
