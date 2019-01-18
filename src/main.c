#include<stm32f0xx.h>

void configure_TIM2(void);

void blink(void) {
    GPIOA->BSRR |= (1 << 5);
    for (int i=0; i < 100000; ++i) __NOP();
    GPIOA->BRR |= (1 << 5);
    for (int i=0; i < 100000; ++i) __NOP();
}

int main() {
    /* setup GPIO */
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIOA->MODER |= GPIO_MODER_MODER5_0;
    /* setup the timer */
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2->ARR = (uint32_t)0x00FFFFFF;
    TIM2->CR1 |= TIM_CR1_OPM;
    TIM2->DIER |= TIM_DIER_UIE;
    NVIC_EnableIRQ(TIM2_IRQn);
    NVIC_SetPriority(TIM2_IRQn, 0);
    TIM2->CR1 |= TIM_CR1_CEN;
    /* start timer */
    TIM2->EGR |= TIM_EGR_UG;

    while (1) {}

    return 0;
}

