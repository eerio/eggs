/* Miscellaneous, commonly used functions and procedures
 *
 * author: Paweł Balawender
 * github.com@eerio
 */
#include<common.h>

/* Macros for the User LED, LD2, pin 5 @ GPIO port A */
#define LED_PORT (GPIOA)
#define LED_PIN (5U)
#define LED_ON() (LED_PORT->BSRR |= (1 << LED_PIN))
#define LED_OFF() (LED_PORT->BRR |= (1 << LED_PIN))
#define LED_TOGGLE() (LED_PORT->ODR ^= (1 << LED_PIN))

uint8_t SPI_RX_buffer[SPI_RX_BUFFER_SIZE] = {0};
uint8_t SPI_TX_buffer[SPI_TX_BUFFER_SIZE] = {0};
uint32_t SPI_RX_ind = 0;

void send_command(int cmd, void* msg) {
    __asm__(
        "mov r0, %[cmd];"
        "mov r1, %[msg];"
        "bkpt #0xAB"
          :
          : [cmd] "r" (cmd), [msg] "r" (msg)
          : "r0", "r1", "memory"
          );
}

