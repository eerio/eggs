#include<os.h>

void init_os(void) {

}

void init_task(void (*handler)(void)) {
    handler();
}

void start_os(void) {

}

