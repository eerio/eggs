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

typedef unsigned int bool;

/* Task Control Block type */
typedef struct {
    void *sp;
    bool ready;
    bool cond;
    bool kill_flag;
} TCB;

/* Basic stack frame structure
 * Since Cortex-M0 has a Full Descending stack and NVIC wants R0 at its SP
 * and xPSR at offset SP+0x1C, the order of these fields cannot be
 * changed or reversed - it's natural in Cortex-M0 environment this way
 */
static struct {
    uint32_t r0; /* Lowest address */
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t LR;
    uint32_t PC;
    uint32_t xPSR; /* Highest address, <StackFrame> + 0x1C */
} StackFrame = {0, 0, 0, 0, 0, (uint32_t)LoopForever, 0, DEFAULT_XPSR};

/* Table of all the tasks initialied */
static struct {
    TCB tasks[MAX_TASKS];
    unsigned int current_task_num;
    unsigned int tasks_num;
} TaskTable = {0};

/* TCBs of the currently executing task and the next one */
TCB *current_tcb, *next_tcb;


void kill(uint8_t id) {TaskTable.tasks[id].kill_flag = 1;}

void timedwait(uint8_t id) {
    TaskTable.tasks[id].ready = 0;
}

void signal(uint8_t id) {TaskTable.tasks[id].ready = 1;}
void yield(void) {SCB->ICSR |= SCB_ICSR_PENDSTSET_Msk;}

void init_os(void) {
    /* Lowest priority is obligatory for IRQ handlers to execute casually */
    NVIC_EnableIRQ(SysTick_IRQn);
    NVIC_EnableIRQ(PendSV_IRQn);
    NVIC_SetPriority(SysTick_IRQn, 0xFF);
    NVIC_SetPriority(PendSV_IRQn, 0xFF);
}

void init_task(void (*handler)(bool*)) {
    /* Add new TCB to the table */
    TCB *tcb = &TaskTable.tasks[TaskTable.tasks_num];
    
    /* Allocate memory for the stack and verify it */
    void *new_stack = calloc(STACK_SIZE, 1U);
    if (!new_stack) while(1) { /* UNABLE TO ALLOCATE A NEW STACK */ }
    
    /* Initialize TCB, leave 32 bytes for 8 initial register values */
    tcb->sp = new_stack + STACK_SIZE - sizeof StackFrame - 32;

    /* Mark the thread as ready to execute */
    tcb->ready = 1;
    tcb->cond = 0;
    tcb->kill_flag = 0;

    /* Initialize stack */
    StackFrame.PC = (uint32_t)handler;
    StackFrame.r0 = (uint32_t)(&tcb->kill_flag);
    memcpy(tcb->sp, &StackFrame, sizeof StackFrame);

    /* Update TaskTable metadata */
    TaskTable.tasks_num++;
}

void start_os(uint32_t period) {
    /* Set the default TCB
     * We use the last task, so it gets stacked (even though it's already
     * here), and then task #0 begins to execute. The advantage of
     * this solution is that #0's stack doesn't get spoiled by some
     * unnecessary calls to its handler
     */
    TaskTable.current_task_num = TaskTable.tasks_num - 1;
    current_tcb = &TaskTable.tasks[TaskTable.current_task_num];

    /* Set SysTick interrupt period */
    SysTick_Config(period);

    /* Set the appriopriate Process Stack Pointer, force the processor
     * to use it as its SP and flush the pipeline of the processor
     */
    __set_PSP((uint32_t)current_tcb->sp);
    __set_CONTROL(0x02);
    __ISB();

    /* Fetch the handler from current TCB and call it
     * 0x18 is the offset from the Frame Stack beginning and the PC val
     * It corresponds to 6 registers that we want to omit:
     * r0, r1, r2, r3, r12, lr
     */
    void (**handler)(bool*) = current_tcb->sp + 0x18;
    (*(*handler))(&current_tcb->kill_flag);
}

void SysTick_Handler(void) {
    /* Update current TCB */
    current_tcb = &TaskTable.tasks[TaskTable.current_task_num];

    /* Fetch next task to execute */
    do {
        TaskTable.current_task_num++;
        if (TaskTable.current_task_num >= TaskTable.tasks_num) {
            TaskTable.current_task_num = 0;
        }
    } while (TaskTable.tasks[TaskTable.current_task_num].ready == 0);

    /* Update next TCB */
    next_tcb = &TaskTable.tasks[TaskTable.current_task_num];

    /* Jump to PendSV for context switching
     * for SCB register documentation: ProgMan, p.78
     * for SCB register definition: core_cm0.h
     */
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

