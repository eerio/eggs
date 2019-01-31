#include<common.h>

void delay (int t) {
    for(int i=0; i < t; ++i) __NOP();
}

void led_on_delay(void) {
    LED_ON();
    delay(100000);
}

void led_off_delay(void) {
    LED_OFF();
    delay(100000);
}

