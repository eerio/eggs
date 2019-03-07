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
#include<spi.h>
#include<sd.h>
#include<pff.h>

void OS_setup(void);

void die(FRESULT rc) {
    while(1) { /* Failed with passed rc */ }
}

int main(void) {
    /* At this point we assume that the stack is initialized,
     * .data segment is copied to SRAM, .bss segment is zero-filled,
     * SystemCoreClock variable is set to current system clock source,
     * internal clocks are initialized and __libc_init_array routine has
     * been called. These things are done by ResetHandler in
     * startup_<device>.s file and SystemInit func in system_<device_fam>.c
     */
    init_sys();

    FATFS fatfs;
    FRESULT rc = pf_mount(&fatfs);
    unsigned int br;
    uint8_t buff[64];

    if (rc) die(rc);

    rc = pf_open("hi.txt");
    if (rc) die(rc);

    rc = 0;
    br = 1;
    while(!rc && br) {
        rc = pf_read(buff, sizeof(buff), &br);
    }
    if (rc) die(rc);
    
    disable_spi();
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

