/* Collection of miscellaneous and commonly-used functions and macros
 *
 * author: Paweł Balawender
 * github.com@eerio
 */
#include<stm32f0xx.h>

/* Send byte by SPI1 */
void SPI_send(uint8_t);

/* Simplest delay routine possible, it counts down n times */
void delay(unsigned int);

/* Handlers. They periodically toggle the User LED with different freq. */
void handler_blinking_fast(void);
void handler_blinking_medium(void);
void handler_blinking_slow(void);

