#include<stm32f0xx.h>


int main() {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIOA->MODER |= GPIO_MODER_MODER5_0;
    GPIOA->BSRR |= (1 << 5);

    while (1) {

    }
    return 0;
}

