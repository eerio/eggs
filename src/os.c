#include<os.h>
#define MAX_TASKS (16U)

typedef struct {
    uint32_t sp;
    void (*handler)(void);
} os_tcb;

struct {
    os_tcb tasks[MAX_TASKS];
    uint32_t cur;
    uint32_t n;
} task_table;

os_tcb *current_tcb;
os_tcb *next_tcb;

void init_os(void) {
    NVIC_EnableIRQ(SysTick_IRQn);
    NVIC_EnableIRQ(PendSV_IRQn);
    /* priority low to allow IRQs to pend normally */
    NVIC_SetPriority(SysTick_IRQn, 0x00);
    NVIC_SetPriority(SysTick_IRQn, 0x00);

    SysTick_Config(SystemCoreClock);
    //NVIC_EnableIRQ(SVC_IRQn);
    //NVIC_SetPriority(SVC_IRQn, 0xFF);
}
void task_finished(void) {while(1){}}

void init_task(void (*handler)(void), uint32_t *p_stack,
        uint32_t stack_size) {
    os_tcb *tcb = &task_table.tasks[task_table.n];
    tcb->handler = handler;
    tcb->sp = (uint32_t)(p_stack + stack_size - 16);
    p_stack[stack_size-1] = 0x01000000;
    p_stack[stack_size-2] = (uint32_t)handler;
    p_stack[stack_size-3] = (uint32_t)&task_finished;
}

void start_os(void) {

}

void SysTick_Handler(void) {
    // 1. update
    current_tcb = &task_table.tasks[task_table.cur];
    // 2. get next task to run

    task_table.cur++;
    if (task_table.cur >= task_table.n) task_table.cur = 0;
    next_tcb = &task_table.tasks[task_table.cur];

    // 3. switch context
    // this def is in core_cm0.h; doc: ProgMan, p.78
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

