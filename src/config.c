/* System configuration procedures
 *
 * author: Paweł Balawender
 * github.com@eerio
 */
#include<stm32f0xx.h>
#include<config.h>

/* It's designed specifically for STM32F091RC development board */
void init_sys(void) {
    /* Configure the User LED, LD2 */
    /* TODO: Check whether every STM32 board has a User LED
     * TODO: Check whether the User LED is always at GPIOA pin 5
     * TODO: Change this code so it's portable across STM32 boards
     */
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIOA->MODER |= GPIO_MODER_MODER5_0;
}

