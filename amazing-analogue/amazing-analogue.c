#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"


#ifndef LED_PIN_FIRST 
#define LED_PIN_FIRST 18
#endif
#ifndef LED_PIN_SECOND
#define LED_PIN_SECOND 19
#endif
#ifndef LED_PIN_THIRD
#define LED_PIN_THIRD 20
#endif

#ifndef POT_PIN
#define POT_PIN 27
#endif


void pico_led_init(void)
{
    gpio_init(LED_PIN_FIRST);
    gpio_set_dir(LED_PIN_FIRST, GPIO_OUT);
    
    gpio_init(LED_PIN_SECOND);
    gpio_set_dir(LED_PIN_SECOND, GPIO_OUT);
    
    gpio_init(LED_PIN_THIRD);
    gpio_set_dir(LED_PIN_THIRD, GPIO_OUT);
}


void pico_set_led(bool led_on)
{
    gpio_put(LED_PIN_FIRST, led_on);
    gpio_put(LED_PIN_SECOND, led_on);
    gpio_put(LED_PIN_THIRD, led_on);
}


int main()
{
    stdio_init_all();

    pico_led_init();
    adc_init();
    adc_gpio_init(POT_PIN); // GPIO 27 is ADC1
    adc_select_input(1); // Select ADC1

    gpio_set_function(LED_PIN_THIRD, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(LED_PIN_THIRD);

    pwm_set_wrap(slice_num, 65535);
    pwm_set_chan_level(slice_num, PWM_CHAN_A, 0);
    pwm_set_enabled(slice_num, true);

    while (true)
    {
        uint16_t value = adc_read();

        printf("ADC Value: %d\n", value);

        pwm_set_chan_level(slice_num, PWM_CHAN_A, value);

        sleep_ms(1000);
    } 
}
