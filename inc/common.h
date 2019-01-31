#include<stm32f0xx.h>

#define LD2_PORT (GPIOA)
#define LD2_PIN (5U)
#define LED_ON() (LD2_PORT->BSRR |= (1 << LD2_PIN))
#define LED_OFF() (LD2_PORT->BRR |= (1 << LD2_PIN))

void led_on_delay(void);
void led_off_delay(void);
void delay(int);

