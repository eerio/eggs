#include<common.h>
#include<config.h>
#include<os.h>


int main() {
    init_sys();
    init_os();

    uint32_t st_1[1024];
    uint32_t st_2[1024];
    init_task(led_on_delay, st_1, 1024);
    init_task(led_off_delay, st_2, 1024);
    start_os();
    
    while(1) {}
    return 0;
}
