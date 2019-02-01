#include<stm32f0xx.h>

void init_os(void);
void init_task(void (*handler)(void), uint32_t*, uint32_t);
void start_os(void);

