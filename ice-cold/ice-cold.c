#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/i2c.h"
#include "ws2812.pio.h"


#define RGB_PIN 16
#define NUM_LEDS 12
#define IS_RGBW false
#define I2C_PORT i2c1
#define SDA_PIN 14
#define SCL_PIN 15
#define DHT20_ADDR 0x38


bool dht20_read(float *temp, float *humidity, bool *crc_ok)
{
    uint8_t cmd [3] = {0xAC, 0x33, 0x00};
    uint8_t data[7];

    if(i2c_write_blocking(I2C_PORT, DHT20_ADDR, cmd, 3, false) < 3)
        return false;

    sleep_ms(80);
    
    if(i2c_read_blocking(I2C_PORT, DHT20_ADDR, data, 7, false) < 0)
        return false;

    *crc_ok = true;

    uint32_t raw_humidity = ((uint32_t)data[1] << 12) | ((uint32_t)data[2] << 4) | (data[3] >> 4);

     uint32_t raw_temp = ((uint32_t)(data[3] & 0x0F) << 16) |
                     ((uint32_t)data[4] << 8) |
                     data[5];
                
    *humidity = (raw_humidity * 100.0f) / 1048576.0f;
    *temp = (raw_temp * 200.0f) / 1048576.0f - 50.0f;
    return true;
}

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

// Show temperature on the LED Ring
// C doesn't have a dict, so we map 14-25C to 0-11 LEDs
void show_temperature_led(int temp)
{
    int led = temp - 14;
    if (led < 0) led = 0;
    if (led >11) led = 11;

    for (int i = 0; i < NUM_LEDS; i++)
    {
        if (i <= led)
        {
            put_pixel(pio0, 0, urgb_u32(0, 0, 255)); 
        }
        else
        {
            put_pixel(pio0, 0, urgb_u32(0, 0, 0)); 
        }
    }
}


int main()
{
    stdio_init_all();

    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

     PIO pio = pio0;
    uint sm = 0;

    uint offset = pio_add_program(pio, &ws2812_program);

    ws2812_program_init(
        pio,
        sm,
        offset,
        RGB_PIN,
        800000,
        IS_RGBW
    );

    sleep_ms(200);
    while (true) {
        float temp, rh;
        bool crc_ok;


         if (dht20_read(&temp, &rh, &crc_ok))
        {
            printf("Temperature: %.2f C\n", temp);
            printf("Humidity: %.2f %%\n", rh);
            printf("CRC OK: %s\n\n", crc_ok ? "true" : "false");
            show_temperature_led((int)temp);
        }
        else
        {
            printf("DHT20 read failed:( \n\n");
        }

        sleep_ms(5000);
    }
}
