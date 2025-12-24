#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define I2C_PORT i2c1
#define SDA_PIN 14
#define SCL_PIN 15
#define LCD_ADDRESS 0x27
#define DHT20_ADDR 0x38

#define LCD_BACKLIGHT 0x08
#define LCD_ENABLE 0x04
#define LCD_RS 0x01


void i2c_write_byte(uint8_t val) {
    i2c_write_blocking(I2C_PORT, LCD_ADDRESS, &val, 1, false);
    sleep_us(50);
}

void lcd_pulse_enable(uint8_t data) {
    i2c_write_byte(data | LCD_ENABLE);
    sleep_us(1);
    i2c_write_byte(data & ~LCD_ENABLE);
    sleep_us(50);
}

void lcd_write4(uint8_t data) {
    i2c_write_byte(data | LCD_BACKLIGHT);
    lcd_pulse_enable(data | LCD_BACKLIGHT);
}

void lcd_send(uint8_t value, uint8_t mode) {
    uint8_t high = (value & 0xF0) | mode;
    uint8_t low  = ((value << 4) & 0xF0) | mode;
    lcd_write4(high);
    lcd_write4(low);
}

void lcd_command(uint8_t cmd) {
    lcd_send(cmd, 0);
}

void lcd_data(uint8_t data) {
    lcd_send(data, LCD_RS);
}

void lcd_init() {
    sleep_ms(50);

    lcd_write4(0x30);
    sleep_ms(5);
    lcd_write4(0x30);
    sleep_us(150);
    lcd_write4(0x30);
    lcd_write4(0x20); 

    lcd_command(0x28); 
    lcd_command(0x0C); 
    lcd_command(0x06); 
    lcd_command(0x01); 
    sleep_ms(2);
}

void lcd_puts(const char *s) {
    while (*s) {
        lcd_data(*s++);
    }
}

void lcd_clear(void){
    lcd_command(0x01);
    sleep_ms(2);
}

void lcd_set_cursor(uint8_t col, uint8_t row)
{
    static const uint row_offsets[] = { 0x00, 0x40 };
    lcd_command(0x80 | (col + row_offsets[row]));
}

void dht20_trigger_measurement() {
    uint8_t cmd[3] = {0xAC, 0x33, 0x00};
    i2c_write_blocking(I2C_PORT, DHT20_ADDR, cmd, 3, false);
    sleep_ms(80);
}

bool dht20_read(float *temperature, float *humidity) {
    uint8_t data[7];

    if (i2c_read_blocking(I2C_PORT, DHT20_ADDR, data, 7, false) != 7)
        return false;

    uint32_t rh_raw =
        ((uint32_t)data[1] << 12) |
        ((uint32_t)data[2] << 4) |
        ((data[3] >> 4) & 0x0F);

    uint32_t t_raw =
        ((uint32_t)(data[3] & 0x0F) << 16) |
        ((uint32_t)data[4] << 8) |
        data[5];

    *humidity = (rh_raw * 100.0f) / 1048576.0f;
    *temperature = (t_raw * 200.0f) / 1048576.0f - 50.0f;

    return true;
}

int main() {
    stdio_init_all();

    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    lcd_init();

    lcd_set_cursor(0, 0);
    lcd_puts("Temp:");
    lcd_set_cursor(0, 1);
    lcd_puts("Humidity:");

    char buffer[8];


   while (true) {
        float temp, hum;

        dht20_trigger_measurement();

        if (dht20_read(&temp, &hum)) {
            snprintf(buffer, sizeof(buffer), "%4.1f", temp);
            lcd_set_cursor(12, 0);
            lcd_puts(buffer);

            snprintf(buffer, sizeof(buffer), "%4.1f", hum);
            lcd_set_cursor(12, 1);
            lcd_puts(buffer);
        }

        sleep_ms(5000);
    }
}