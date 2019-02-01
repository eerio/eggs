#include<common.h>

void delay (volatile int t) {
    while(t > 0) --t;
}

void blinking(int t) {
    LED_ON();
    delay(t);
    LED_OFF();
    delay(t);
}

void hand_blink_fast(void) {
    while(1) {
        __disable_irq();
        GPIOA->ODR ^= (1 << LED_PIN);
        __enable_irq();
        delay(100000);
    }
}

void hand_blink_med(void) {
    while(1) {
        __disable_irq();
        GPIOA->ODR ^= (1 << LED_PIN);
        __enable_irq();
        delay(50000);
    }
}

void hand_blink_slow(void) {
    while(1) {
        __disable_irq();
        LED_OFF();
        __enable_irq();
        delay(35000);
    }
}

