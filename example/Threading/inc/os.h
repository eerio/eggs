/* Prototypes of core Operating System procedures and functions
 *
 * author: Pawel Balawender
 * https://github.com/eerio/eggs.git
 */

/* Infinite loop function, implemented in startup_stm32f091xc.s file */
void LoopForever(void);

/* Initialize OS environment - configure interrupts, create TCBs etc. */
void init_os(void);

/* Initialize new task for a future thread - allocate memory, prepare stack */
void init_task(void (*)(void));

/* Start execution of the tasks in the queue */
void start_os(void);

