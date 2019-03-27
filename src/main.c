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

#define LED_PIN (5U)
#define LED_ON() (GPIOA->BSRR |= (1U << LED_PIN))
#define LED_OFF() (GPIOA->BRR |= (1U << LED_PIN))
#define LED_TOG() (GPIOA->ODR ^= (1U << LED_PIN))

typedef unsigned int bool;

void die(FRESULT rc);
void test_pff(void);

BYTE buff[64];

void handler_blink(bool*);
void handler_still(bool*);

void EXTI4_15_IRQHandler(void) {
    if (EXTI->PR & EXTI_PR_PIF13) {
        delay(SystemCoreClock / 8);
        LED_TOG();  
        EXTI->PR |= EXTI_PR_PIF13;
    }
}

int main(void) {
    /* At this point we assume that the stack is initialized,
     * .data segment is copied to SRAM, .bss segment is zero-filled,
     * SystemCoreClock variable is set to current system clock source,
     * internal clocks are initialized and __libc_init_array routine has
     * been called. These things are done by ResetHandler in
     * startup_<device>.s file and SystemInit func in system_<device_fam>.c
     */
    /* Set LD2 to output mode */
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIOA->MODER |= GPIO_MODER_MODER5_0;

    /* Enable clock for GPIO port C*/
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    /* Select push-pull input mode with internal pull-down resistor for B1 */
    GPIOC->MODER &= ~GPIO_MODER_MODER13;
    GPIOC->OTYPER &= ~GPIO_OTYPER_OT_13;
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR13;
    GPIOC->PUPDR |= GPIO_PUPDR_PUPDR13_1;

    /* Select GPIOC as interrupt source on EXTI line 13 */
    SYSCFG->EXTICR[3] |= (0b0010 << SYSCFG_EXTICR4_EXTI13_Pos);
    /* Mask interrupts on EXTI line 13 */
    EXTI->IMR |= EXTI_IMR_MR13;
    /* Enable rising edge trigger for exti line 13 */
    EXTI->RTSR |= EXTI_RTSR_RT13;
    /* Enable highest-priority interrupt in NVIC */
    NVIC_EnableIRQ(EXTI4_15_IRQn);
    NVIC_SetPriority(EXTI4_15_IRQn, 0);

    while(1);

    init_sys();
    init_os();
    init_task(handler_blink);
    init_task(handler_still);
    start_os(SystemCoreClock);

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

