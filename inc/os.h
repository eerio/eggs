/* Prototypes of core Operating System procedures and functions
 *
 * author: Paweł Balawender
 * github.com@eerio
 */
#include<stm32f0xx.h>

typedef unsigned int bool;

/* Infinite loop function, implemented in startup_stm32f091xc.s file */
void LoopForever(void);

/* Initialize OS environment - configure interrupts, create TCBs etc. */
void init_os(void);

/* Initialize new task for a future thread - allocate memory, prepare stack */
void init_task(void (*)(bool*));

/* Start execution of the tasks in the queue */
void start_os(uint32_t);

void kill(uint8_t);
void timedwait(uint8_t);
void signal(uint8_t);
void yield(void);

