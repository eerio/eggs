#include<os.h>
#define MAX_TASKS (16U)

typedef struct {
    uint32_t sp;
    void (*handler)(void);
} os_task;

struct {
    os_task tasks[MAX_TASKS];
    int cur;
    int size;
} task_table;

void init_os(void) {
    NVIC_EnableIRQ(SysTick_IRQn);
    NVIC_EnableIRQ(PendSV_IRQn);
    NVIC_SetPriority(SysTick_IRQn, 0x00);
    NVIC_SetPriority(SysTick_IRQn, 0xFF);

    SysTick_Config(SystemCoreClock);
    //NVIC_EnableIRQ(SVC_IRQn);
    //NVIC_SetPriority(SVC_IRQn, 0xFF);
}

void init_task(void (*handler)(void), uint32_t *p_stack, uint32_t stack_size) {
    //if (++task_table.size > MAX_TASKS) return;

    //os_task *task = &task_table.tasks[task_table.size - 1];
    /*task->sp = (uint32_t)(p_stack + stack_size - 16);
    task->handler = handler;
    p_stack[stack_size - 1] = 0x01000000;
    p_stack[stack_size - 2] = (uint32_t)handler;
    p_stack[stack_size - 3] = 0x01000000;*/
}

void start_os(void) {

}

void SysTick_Handler(void) {
    // 1. get next task to run
    //task_table.cur++;
    //if (task_table.cur >= task_table.n) task_table.cur = 0;
    //next_task = &task_table.tasks[task_table.cur];
    // 2. switch context
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

