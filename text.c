
#include "pico.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "pico/stdlib.h"
#include "picoOled.h"
#include "image.h"

#define PICO_LED 25
#define DEBUG_TRIG 22

#define UINP0 10
#define UINP1 7
#define UINP2 6


t_OledParams g_myOled;

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

    gpio_init(DEBUG_TRIG);
    gpio_set_dir(DEBUG_TRIG, GPIO_OUT);
    gpio_put(DEBUG_TRIG, 0);

    gpio_init(UINP0);
    gpio_set_dir(UINP0, GPIO_IN);

    gpio_init(UINP1);
    gpio_set_dir(UINP1, GPIO_IN);

    gpio_init(UINP2);
    gpio_set_dir(UINP2, GPIO_IN);


}

void waitUinp1(void){
uint8_t inpState = 1;

    while (1){
        inpState = gpio_get(UINP1);
        if (inpState == 0) return;
    }
}



int main(){

char msg[] = "\n\n\nHello, world!";


    stdio_init_all();
    pinConfig();
    gpio_put(PICO_LED, 0);


    waitUinp1();

    gpio_put(PICO_LED, 1);

    printf("Hello\n");

    configOled(&g_myOled);

    oledSetTTYMode(&g_myOled, true);
    oledWriteStr(&g_myOled, (const uint8_t *)msg, strlen(msg));

    oledDraw_line(&g_myOled, 0, 0, 127, 63, WHITE);
    oledDisplay(&g_myOled);

    oledBmp_show_image(&g_myOled, image_data, image_size);
    oledDisplay(&g_myOled);

    while (true){}

}