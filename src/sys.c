/* System configuration procedures
 * note: they're designed specifically for STM32F091RC development board
 *
 * author: Paweł Balawender
 * github.com@eerio
 */
#include<stm32f0xx.h>
#include<config.h>
#include<spi.h>
#include<sd.h>

void init_sys(void) {
    /* init_leds(); */
    setup_spi();
}

void quit_sys(void) {
    disable_spi();
}

