/* Miscellaneous, commonly used functions and procedures
 *
 * author: Pawel Balawender
 * https://github.com/eerio/eggs.git
 */
#include<stm32f0xx.h>
#include<common.h>

/* Macros for the User LED, LD2, pin 5 @ GPIO port A */
#define LED_PORT (GPIOA)
#define LED_PIN (5U)
#define LED_ON() (LED_PORT->BSRR |= (1 << LED_PIN))
#define LED_OFF() (LED_PORT->BRR |= (1 << LED_PIN))
#define LED_TOGGLE() (LED_PORT->ODR ^= (1 << LED_PIN))

void delay (unsigned int time) {
    while (time > 0) {--time;}
}

void handler_blinking_fast(void) {
    while(1) {
        GPIOA->ODR ^= (1 << 5);
        delay(100000);
    }
}

void handler_blinking_medium(void) {
    while(1) {
        GPIOA->ODR ^= (1 << 6);
        delay(50000);
    }
}

void handler_blinking_slow(void) {
    while(1) {
        GPIOA->ODR ^= (1 << 7);
        delay(200000);
    }
}

