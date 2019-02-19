/* Prorotypes for SPI init procedures, communication abstractions etc.
 *
 * author: Paweł Balawender
 * github.com@eerio
 */
#include<stm32f0xx.h>

/* Send a sequence of bytes by the SPI interface */
void SPI_send(unsigned int, uint8_t []);

/* Initialize SPI peripheral */
void setup_spi(void);

