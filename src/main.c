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
#include<sys.h>
#include<pff.h>
#include<delay.h>
#include<os.h>

#define LED_PIN (9U)
#define LED_ON() (GPIOA->BSRR |= (1U << LED_PIN))
#define LED_OFF() (GPIOA->BRR |= (1U << LED_PIN))

typedef unsigned int bool;

void die(FRESULT rc);
void test_pff(void);

BYTE buff[64];

void handler_blink(bool*);
void handler_still(bool*);

int main(void) {
    /* At this point we assume that the stack is initialized,
     * .data segment is copied to SRAM, .bss segment is zero-filled,
     * SystemCoreClock variable is set to current system clock source,
     * internal clocks are initialized and __libc_init_array routine has
     * been called. These things are done by ResetHandler in
     * startup_<device>.s file and SystemInit func in system_<device_fam>.c
     */
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIOA->MODER |= GPIO_MODER_MODER9_0;
    init_sys();/*
    init_os();
    init_task(handler_blink);
    init_task(handler_still);
    start_os(SystemCoreClock);*/

    test_pff();
    quit_sys();

    /* Main thread after return from the main function goes to an infinite
     * loop in the startup_stm32f091xc.s file */
    return 0;
}

void handler_blink(bool *kill_flag) {
    while(*kill_flag == 0) {
        timedwait(1);
        for (int i=0; i < 100; ++i) {
            LED_ON();
            delay(100000);
            LED_OFF();
            delay(100000);
        }
        signal(1);
        yield();
    }
}

void handler_still(bool *kill_flag) {
    while(*kill_flag == 0) {
        LED_ON();
    }
}

void die (		/* Stop with dying message */
	FRESULT rc	/* FatFs return value */
)
{
    quit_sys();
	for (;;) ;
}

void test_pff(void) {
	FATFS fatfs;			/* File system object */
	DIR dir;				/* Directory object */
	FILINFO fno;			/* File information object */
	UINT br, i;
	
    FRESULT rc = pf_mount(&fatfs);
	if (rc) die(rc);

	rc = pf_open("hi.txt");
	if (rc) die(rc);

	for (;;) {
		rc = pf_read(buff, sizeof(buff), &br);	/* Read a chunk of file */
		if (rc || !br) break;			/* Error or end of file */
	}
	if (rc) die(rc);

#if PF_USE_WRITE
	rc = pf_open("WRITE.TXT");
	if (rc) die(rc);

	for (;;) {
		rc = pf_write("Hello world!\r\n", 14, &bw);
		if (rc || !bw) break;
	}
	if (rc) die(rc);

	rc = pf_write(0, 0, &bw);
	if (rc) die(rc);
#endif

#if PF_USE_DIR
	rc = pf_opendir(&dir, "");
	if (rc) die(rc);

	for (;;) {
		rc = pf_readdir(&dir, &fno);	/* Read a directory item */
		if (rc || !fno.fname[0]) break;	/* Error or end of dir */
	}
	if (rc) die(rc);
#endif

}

