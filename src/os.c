/* Core methods and data structures required by the OS to work properly
 *
 * author: Paweł Balawender
 * github.com@eerio
 */
#include<stdlib.h> /* malloc */
#include<string.h> /* memset, memcpy */
#include<os.h>

#define MAX_TASKS (16U)
#define STACK_SIZE (1024U)


/* It's implemented in startup_stm32f091xc.s file */
void LoopForever(void);

/* Task Control Block type */
typedef struct {
    void *sp;
    void (*handler)(void);
} TCB;

/* Basic stack frame structure
 * Assign 0xC1000000 to xPSR by default - it's the value that my
 * board starts with (gdb shows it), along with*/
static struct {
    uint32_t xPSR;
    uint32_t PC;
    uint32_t LR;
    uint32_t r12;
    uint32_t r3;
    uint32_t r2;
    uint32_t r1;
    uint32_t r0;
} StackFrame = {0xC1000000U, 0, (uint32_t)LoopForever,
    1000, 1001, 1002, 1003, 1004};

/* Table of all the tasks initialied */
static struct {
    TCB tasks[MAX_TASKS];
    unsigned int current_task_num;
    unsigned int tasks_num;
} TaskTable;

/* Pointers to TCBs of the currently executing task and the next one */
/* TODO: why tf i made them pointers, they can be direct, dont they?*/
TCB *current_tcb, *next_tcb;


void init_os(void) {
    /*memset(&TaskTable, 0, sizeof TaskTable);*/
    /* Lowest priority is obligatory for IRQ handlers to execute casually */
    NVIC_EnableIRQ(SysTick_IRQn);
    NVIC_EnableIRQ(PendSV_IRQn);
    NVIC_SetPriority(SysTick_IRQn, 0x00);
    NVIC_SetPriority(PendSV_IRQn, 0xFF);

    /* Set the default TCB */
    current_tcb = &TaskTable.tasks[0];
}

void init_task(void (*handler)(void)) {
    /* Add new TCB to the table */
    TCB *tcb = &TaskTable.tasks[TaskTable.tasks_num];

    /* Allocate memory for the stack */
    void *new_stack = malloc(STACK_SIZE);
    
    /* Initialize stack with a proper stack frame to pop from it */
    StackFrame.PC = (uint32_t)handler;
    memcpy(new_stack, &StackFrame, sizeof StackFrame);

    /* Initialize TCB */
    tcb->sp = new_stack;
    tcb->handler = handler;

    /* Update TaskTable metadata */
    TaskTable.tasks_num++;
}

void start_os(void) {
    /* Start SysTick */
    SysTick_Config(SystemCoreClock);
    /* Now set Stack Pointer properly, so it's get saved in the
     * Stack Frame of proper task:
     * - Set appropriate Stack Pointer
     * - Switch SP to use Process Stack Pointer
     * - Flush processor pipeline for it to use the new SP
     */
    __set_PSP((uint32_t)current_tcb->sp);
    __set_CONTROL(2);
    __ISB();

    /* Start execution of the first task */
    current_tcb->handler();
}

void SysTick_Handler(void) {
    /* Update current TCB's pointer */
    current_tcb = next_tcb;

    /* Fetch next task to execute */
    if (++TaskTable.current_task_num >= TaskTable.tasks_num) {
        TaskTable.current_task_num = 0;
    }

    /* Update next TCB's pointer */
    next_tcb = &TaskTable.tasks[TaskTable.current_task_num];

    /* Jump to PendSV for context switching
     * for SCB register documentation: ProgMan, p.78
     * for SCB register definition: core_cm0.h
     */
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

