/* Prototypes of core Operating System procedures and functions
 *
 * author: Paweł Balawender
 * github.com@eerio
 */
#include<stm32f0xx.h>

/* Initialize OS environment - configure interrupts, create TCBs etc. */
void init_os(void);

/* Initialize new task for a future thread - allocate memory, prepare stack */
void init_task(void (*)(void));

/* Start execution of the tasks in the queue
 * This function is implemented in start_os.s file
 */
void start_os(void);

