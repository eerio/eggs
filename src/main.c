#include<common.h>
#include<config.h>
#include<os.h>


int main() {
    init_sys();
    init_os();
    init_task(led_on_delay);
    init_task(led_off_delay);
    start_os();
    
    while(1) {}
    return 0;
}

