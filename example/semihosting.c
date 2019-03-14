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
#include<stdint.h> /* uint32_t */
#include<semihosting.h>


int main(void) {
    /* At this point we assume that the stack is initialized,
     * .data segment is copied to SRAM, .bss segment is zero-filled,
     * SystemCoreClock variable is set to current system clock source,
     * internal clocks are initialized and __libc_init_array routine has
     * been called. These things are done by ResetHandler in
     * startup_<device>.s file and SystemInit func in system_<device_fam>.c
     */
    /* Message to send */
    const char s[] = "Hello!\n";
    /* 2: STDERR file descriptor */
    uint32_t m[] = {2, (uint32_t)s, sizeof(s) / sizeof(char) - 1};
    /* 0x05: "write" syscall */
    send_command(0x05, m);

    /* Main thread after return from the main function goes to an infinite
     * loop in the startup_stm32f091xc.s file */
    return 0;
}

