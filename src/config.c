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

/* Enable GPIOA and set the correct pin modes for PA5, PA6 and PA7 */
void init_leds(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIOA->MODER |= GPIO_MODER_MODER5_0;
    GPIOA->MODER |= GPIO_MODER_MODER6_0;
    GPIOA->MODER |= GPIO_MODER_MODER7_0;
}

