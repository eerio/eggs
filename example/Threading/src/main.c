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
#include<config.h>
#include<os.h>
#include<common.h>


int main(void) {
    /* Configure board's peripherals */
    init_sys();
    /* Setup the operating system environment */
    init_os();

    /* Add threads to perform */
    init_task(handler_blinking_fast);
    init_task(handler_blinking_medium);
    init_task(handler_blinking_slow);

    /* Start executing the threads */
    start_os();

    /* Main thread after return from the main function goes to an infinite
     * loop in the startup_stm32f091xc.s file */
    return 0;
}

