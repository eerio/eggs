#include<stm32f0xx.h>

#define LED_PORT (GPIOA)
#define LED_PIN (5U)
#define LED_ON() (LED_PORT->BSRR |= (1 << LED_PIN))
#define LED_OFF() (LED_PORT->BRR |= (1 << LED_PIN))

void delay(int);
void blinking(int);
void hand_blink_fast(void);
void hand_blink_med(void);
void hand_blink_slow(void);

