#include "ds18b20.h"
#include "driver/gpio.h"
#include "esp_rom_sys.h"

#define DS_PIN GPIO_NUM_4

#define AVG_WINDOW 5   // liczba próbek

static float samples[AVG_WINDOW];
static int index = 0;
static int count = 0;

//Reset 1-Wire bus and check for presence pulse
static int ow_reset(void)
{
    gpio_set_direction(DS_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(DS_PIN, 0);
    esp_rom_delay_us(480);

    gpio_set_direction(DS_PIN, GPIO_MODE_INPUT);
    esp_rom_delay_us(70);

    int presence = !gpio_get_level(DS_PIN);
    esp_rom_delay_us(410);

    return presence;
}

//Write a single bit to 1-Wire bus
static void ow_write_bit(int b)
{
    gpio_set_direction(DS_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(DS_PIN, 0);
    esp_rom_delay_us(b ? 6 : 60);

    gpio_set_direction(DS_PIN, GPIO_MODE_INPUT);
    esp_rom_delay_us(b ? 64 : 10);
}
//Read a single bit from 1-Wire bus
static int ow_read_bit(void)
{
    int r;
    gpio_set_direction(DS_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(DS_PIN, 0);
    esp_rom_delay_us(6);

    gpio_set_direction(DS_PIN, GPIO_MODE_INPUT);
    esp_rom_delay_us(9);
    r = gpio_get_level(DS_PIN);
    esp_rom_delay_us(55);
    return r;
}

//Write a byte to 1-Wire bus
static void ow_write_byte(uint8_t b)
{
    for (int i = 0; i < 8; i++) {
        ow_write_bit(b & 1);
        b >>= 1;
    }
}

//Read a byte from 1-Wire bus
static uint8_t ow_read_byte(void)
{
    uint8_t r = 0;
    for (int i = 0; i < 8; i++) {
        r >>= 1;
        if (ow_read_bit())
            r |= 0x80;
    }
    return r;
}

//Initialize 1-Wire bus
void ds18b20_init(void)
{
    gpio_set_pull_mode(DS_PIN, GPIO_PULLUP_ONLY);
}

//Read temperature from DS18B20 sensor
float ds18b20_read(void)
{
    if (!ow_reset())
        return NAN;

    ow_write_byte(0xCC); // SKIP ROM
    ow_write_byte(0x44); // CONVERT T
    esp_rom_delay_us(750000);

    if (!ow_reset())
        return NAN;

    ow_write_byte(0xCC);
    ow_write_byte(0xBE); // READ SCRATCHPAD

    uint8_t lsb = ow_read_byte();
    uint8_t msb = ow_read_byte();

    int16_t raw = (msb << 8) | lsb;
    return raw / 16.0;
}

//Add a new sample to the moving average
void avg_add_sample(float value)
{
    if((value == NAN || value > samples[index]+1 || value < samples[index]-1) && count >= AVG_WINDOW)
        return;
    samples[index] = value;
    index = (index + 1) % AVG_WINDOW;

    if (count < AVG_WINDOW)
        count++;
}

//Get the current moving average
float avg_get()
{
    float sum = 0.0f;

    for (int i = 0; i < count; i++)
        sum += samples[i];

    return sum / count;
}