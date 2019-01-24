#include<stm32f0xx.h>

void led_on(void) {GPIOA->BSRR |= (1 << 5);}
void led_off(void) {GPIOA->BRR |= (1 << 5);}
void delay(int t) {for (int i=0; i < t; ++i) __NOP();}

void thread_1(void) {
    led_on();
    delay(100000);
}

void thread_2(void) {
    led_off();
    delay(100000);
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

