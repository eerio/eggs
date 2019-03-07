/* System configuration procedures
 *
 * author: Pawel Balawender
 * https://github.com/eerio/eggs.git
 */
#include<stm32f0xx.h>
#include<config.h>

/* It's designed specifically for STM32F091RC development board */
void init_sys(void) {
    /* Configure the User LED, LD2 */
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIOA->MODER |= GPIO_MODER_MODER5_0;
    GPIOA->MODER |= GPIO_MODER_MODER6_0;
    GPIOA->MODER |= GPIO_MODER_MODER7_0;
}

