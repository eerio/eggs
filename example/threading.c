/* This is a demonstration of my Operating System
 *
 * It's designed for STM32F091RC development board
 * and you should test this example on it.
 * After compiling the example and programming the binary
 * onto the board, run it and you should see
 * your User LED (LD2, pin 5 @ GPIO port A) blinking
 * at periodically changing frequency
 *
 * author: Paweł Balawender
 * https://github.com/eerio/eggs.git
 */
#include<delay.h>
#include<os.h>

#define LED_ON() (GPIOA->BSRR |= (1 << 5))
#define LED_OFF() (GPIOA->BRR |= (1 << 5))

void handler_blinking_fast(void);
void handler_blinking_medium(void);
void handler_blinking_slow(void);


int main(void) {
    /* Configure board's peripherals */
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIOA->MODER |= GPIO_MODER_MODER5_0;

    /* Setup the operating system environment */
    init_os();

    /* Add threads to perform */
    init_task(handler_blinking_fast);
    init_task(handler_blinking_medium);
    init_task(handler_blinking_slow);

    /* Start executing the threads with SysTick interrupt period = 1s*/
    start_os(SystemCoreClock);

    /* Main thread after return from the main function goes to an infinite
     * loop in the startup_stm32f091xc.s file */
    return 0;
}

void handler_blinking_fast(void) {while(1) LED_ON();}

void handler_blinking_medium(void) {
    while(1) {
        LED_ON();
        delay(50000);
        LED_OFF();
        delay(50000);
    }
}

void handler_blinking_slow(void) {while(1) LED_OFF();}

