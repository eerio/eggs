/* Miscellaneous, commonly used functions and procedures
 *
 * author: Paweł Balawender
 * github.com@eerio
 */
#include<common.h>

/* Macros for the User LED, LD2, pin 5 @ GPIO port A */
#define LED_PORT (GPIOA)
#define LED_PIN (5U)
#define LED_ON() (LED_PORT->BSRR |= (1 << LED_PIN))
#define LED_OFF() (LED_PORT->BRR |= (1 << LED_PIN))
#define LED_TOGGLE() (LED_PORT->ODR ^= (1 << LED_PIN))

/* Send byte by SPI */
void SPI_send(uint8_t data) {
    while (SPI1->SR & SPI_SR_BSY) {}
    GPIOA->ODR &= ~(1 << 4);
    SPI1->DR = data;
    GPIOA->ODR |= (1 << 4);
}

/* TODO: Make it timer- and interruption-based, so it's accurate,
 *          [time-in-seconds-or-minutes-or-sth]-programmable and
 *          another interrupts occuring will not affect it
 * TODO: It should be NOP()-based
 */
void delay (unsigned int time) {
    while (time > 0) {--time;}
}

/* TODO: Shouldn't there be some irq-disables/enables? */
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

