/* Core methods and data structures required by the OS to work properly
 *
 * author: Paweł Balawender
 * github.com@eerio
 */
#include<stm32f0xx.h> /* SCB */
#include<stdlib.h> /* calloc */
#include<string.h> /* memcpy */
#include<os.h>

#define MAX_TASKS (16U)
#define STACK_SIZE (256U)
#define DEFAULT_XPSR (0xC1000000U)

/* Task Control Block type */
typedef struct {
    void *sp;
} TCB;

/* Basic stack frame structure */
static struct {
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t LR;
    uint32_t PC;
    uint32_t xPSR;
} StackFrame = {0, 0, 0, 0, 0, (uint32_t)LoopForever, 0, DEFAULT_XPSR};

/* Table of all the tasks initialied */
static struct {
    TCB tasks[MAX_TASKS];
    unsigned int current_task_num;
    unsigned int tasks_num;
} TaskTable = {0};

/* TCBs of the currently executing task and the next one */
TCB *current_tcb, *next_tcb;


void init_os(void) {
    /* Lowest priority is obligatory for IRQ handlers to execute casually */
    NVIC_EnableIRQ(SysTick_IRQn);
    NVIC_EnableIRQ(PendSV_IRQn);
    NVIC_SetPriority(SysTick_IRQn, 0xFF);
    NVIC_SetPriority(PendSV_IRQn, 0xFF);
}

void init_task(void (*handler)(void)) {
    /* Add new TCB to the table */
    TCB *tcb = &TaskTable.tasks[TaskTable.tasks_num];
    
    /* Allocate memory for the stack and verify it */
    void *new_stack = calloc(STACK_SIZE, 1U);
    if (!new_stack) while(1) { /* UNABLE TO ALLOCATE A NEW STACK */ }
    
    /* Initialize stack */
    StackFrame.PC = (uint32_t)handler;
    memcpy(new_stack, &StackFrame, sizeof StackFrame);

    /* Initialize TCB */
    tcb->sp = new_stack;

    /* Update TaskTable metadata */
    TaskTable.tasks_num++;
}

void start_os(void) {
    /* Set the default TCB
     * We use the last task, so it's get stacked (even though it's already
     * here), and then task #0 begins to execute. The advantage of
     * this solution is that #0's stack doesn't get spoiled by some
     * unnecessary calls to its handler
     */
    TaskTable.current_task_num = TaskTable.tasks_num - 1;
    current_tcb = &TaskTable.tasks[TaskTable.current_task_num];
    
    /* Set SysTick interrupt period to 1s */
    SysTick_Config(SystemCoreClock);

    /* Set the appriopriate Process Stack Pointer, force the processor
     * to use it as its SP and flush the pipeline of the processor
     */
    __set_PSP((uint32_t)current_tcb->sp);
    __set_CONTROL(0x02);
    __ISB();

    /* Fetch the handler from current TCB and call it */
    void (**handler)(void) = current_tcb->sp + 0x18;
    (*(*handler))();
}

void SysTick_Handler(void) {
    /* Update current TCB */
    current_tcb = &TaskTable.tasks[TaskTable.current_task_num];

    /* Fetch next task to execute */
    TaskTable.current_task_num++;
    if (TaskTable.current_task_num >= TaskTable.tasks_num) {
        TaskTable.current_task_num = 0;
    }

    /* Update next TCB */
    next_tcb = &TaskTable.tasks[TaskTable.current_task_num];

    /* Jump to PendSV for context switching
     * for SCB register documentation: ProgMan, p.78
     * for SCB register definition: core_cm0.h
     */
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

