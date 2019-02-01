#include<stm32f0xx.h>
#include<common.h>
#include<config.h>
#include<os.h>


int main(void) {
    init_sys();
    init_os();


    uint32_t st_1[1024];
    uint32_t st_2[1024];
    uint32_t st_3[1024];
    init_task(&hand_blink_fast, st_1, 1024);
    //init_task(&hand_blink_med, st_2, 1024);
    //init_task(hand_blink_slow, st_3, 1024);

    start_os();
    
    while(1) {}
    return 0;
}

