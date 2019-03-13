/* Prorotypes for SPI init procedures, communication abstractions etc.
 *
 * author: Paweł Balawender
 * github.com@eerio
 */
#include<stm32f0xx.h>

/* Initialize SPI peripheral */
void setup_spi(void);

/* Disable SPI */
void disable_spi(void);

void spi_send(uint8_t*);
uint8_t* spi_read(void);

