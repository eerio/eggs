/* This is a demonstration of my Operating System
 *
 * It's designed for STM32F091RC development board
 * and you should test this example on it.
 * After compiling the example and programming the binary
 * onto the board, run it and you should see
 * your User LED (LD2, pin 5 @ GPIO port A) blinking
 * at periodically changing frequency
 *
 * author: Paweł Balawender, github.com/eerio
 *
 */
#include<config.h>
#include<os.h>
#include<common.h>

void OS_setup(void);


int main(void) {
    /* Configure board's peripherals */
    init_sys();

    char msg[] = "Hello!\n";
    while(1) {
        for (int i=0; i < 7; i++) {
            GPIOA->BRR |= (1 << 4);
            SPI1->DR = msg[i];
            GPIOA->BSRR |= (1 << 4);
        }
        delay(250000);
    }

    /* Main thread after return from the main function goes to an infinite
     * loop in the startup_stm32f091xc.s file */
    return 0;
}

void OS_setup(void) {
    /* Setup the operating system environment */
    init_os();

    /* Add threads to perform */
    init_task(handler_blinking_fast);
    init_task(handler_blinking_medium);
    init_task(handler_blinking_slow);

    /* Start executing the threads */
    start_os();
}

