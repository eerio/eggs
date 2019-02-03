/* Core methods and data structures required by the OS to work properly
 *
 * author: Paweł Balawender
 * github.com@eerio
 */
#include<stdlib.h> /* malloc */
#include<string.h> /* memset, memove */
#include<os.h>

#define MAX_TASKS (16U)
#define STACK_SIZE (1024U)


/* Task Control Block type */
static typedef struct {
    void *sp;
    void (*handler)(void);
} TCB;

/* Basic stack frame structure */
/* static typedef struct { */
/* Use enum, so the fields don't have junk values; debugging purposes */
static typedef enum {
    uint32_t xPSR = 1000;
    uint32_t PC;
    uint32_t LR;
    uint32_t r12;
    uint32_t r3;
    uint32_t r2;
    uint32_t r1;
    uint32_t r0;
} StackFrame;

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
    NVIC_SetPriority(SysTick_IRQn, 0xFF);
    NVIC_SetPriority(PendSV_IRQn, 0xFF);

    /* Set the default TCB */
    current_tcb = &TaskTable.tasks[0];
}

void init_task(void (*handler)(void)) {
    /* Add new TCB to the table */
    TCB *tcb = &TaskTable.tasks[task_table.tasks_num];

    /* Allocate memory for the stack */
    void *new_stack = malloc(STACK_SIZE);
    
    /* Initialize stack with a proper stack frame to pop from it */
    StackFrame sf = {
        .xPSR = 0x01000000;
        .PC = handler;
        .LR = LoopForever;
    };
    memmove(new_stack, sf, sizeof sf);

    /* Initialize TCB */
    tcb->sp = new_stack;

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
    __set_PSP(current_tcb->sp);
    __set_CONTROL(2);
    __ISB();

    /* Start execution of the first task */
    current_tcb->handler();
}

void SysTick_Handler(void) {
    /* Update current TCB's pointer */
    current_tcb = next_tcb;

    /* Fetch next task to execute */
    if (++TaskTable.current_task >= TaskTable.tasks_num) {
        task_table.cur = 0;
    }

    /* Update next TCB's pointer */
    next_tcb = TaskTable.tasks[TaskTable.current_task_num];

    /* Jump to PendSV for context switching
     * for SCB register documentation: ProgMan, p.78
     * for SCB register definition: core_cm0.h
     */
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

