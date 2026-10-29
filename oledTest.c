#include "pico.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "hardware/timer.h"
#include "pico/stdlib.h"
#include "picoOled.h"
#include "image.h"

#define PICO_LED 25
#define ADC_PIN 26
#define BUFFER_SIZE 128

t_OledParams g_myOled;
uint16_t buffer[BUFFER_SIZE];

void configOled(t_OledParams *oled){
    oled->i2c         = i2c0;
    oled->SDA_PIN     = 0;
    oled->SCL_PIN     = 1;
    oled->ctlrType    = CTRL_SH1106;
    oled->i2c_address = 0x3C;
    oled->height      = H_64;
    oled->width       = W_132;

    oledI2cConfig(oled);
}

void pinConfig(void){
    gpio_init(PICO_LED);
    gpio_set_dir(PICO_LED, GPIO_OUT);
}

void adcConfig(void){
    adc_init();
    adc_gpio_init(ADC_PIN);
    adc_select_input(0);
}

void shiftLeftBuffer(void) {
    for (int i = 0; i < BUFFER_SIZE - 1; i++) {
        buffer[i] = buffer[i + 1];
    }
}

void drawWave(void) {
    oledClear(&g_myOled, BLACK);

    for (int i = 1; i < BUFFER_SIZE; i++) {
        uint8_t prevY = (buffer[i - 1] * 63) / 4095;
        uint8_t currY = (buffer[i] * 63) / 4095;

        prevY = 63 - prevY;
        currY = 63 - currY;

        oledDraw_line(&g_myOled, i - 1, prevY, i, currY, WHITE);
    }

    oledDisplay(&g_myOled);
}

int main(){

    stdio_init_all();
    pinConfig();
    configOled(&g_myOled);
    adcConfig();

    gpio_put(PICO_LED, 1);

    memset(buffer, 0, sizeof(buffer));

    while (true){

        shiftLeftBuffer();

        uint16_t adc_value = adc_read();
        buffer[BUFFER_SIZE - 1] = adc_value;

        drawWave();

        sleep_us(100);
    }
}
