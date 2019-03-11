/* Collection of miscellaneous and commonly-used functions and macros
 *
 * author: Paweł Balawender
 * github.com@eerio
 */
#include<stm32f0xx.h>

/* 6 bytes SPI buffer */
#define SPI_TX_BUFFER_SIZE (6U)
#define SPI_RX_BUFFER_SIZE (2048U * 12)

extern uint8_t SPI_RX_buffer[], SPI_TX_buffer[];
extern uint32_t SPI_RX_ind;

/* Simplest delay routine possible, it counts down n times */
void delay(unsigned int);

/* Handlers. They periodically toggle the User LED with different freq. */
void handler_blinking_fast(void);
void handler_blinking_medium(void);
void handler_blinking_slow(void);

/* send command to openocd */
void send_command(int, void*);

