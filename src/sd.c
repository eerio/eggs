/* SDHC card initialization procedure and communication abstractions
 *
 * author: Paweł Balawender
 * github.com@eerio
 */
#include<sd.h>
#include<spi.h>
#define SD_SPI (SPI1)
#define SD_GPIO (GPIOA) 

void init_sd(void) {
    /* Save SPI settings */
    uint32_t moder = SD_GPIO->MODER,
             cr1 = SD_SPI->CR1,
             cr2 = SD_SPI->CR2;

    /* Change NSS mode to output */
    SD_GPIO->MODER &= ~GPIO_MODER_MODER4;
    SD_GPIO->MODER |= GPIO_MODER_MODER4_0;
    /* Change NSS management to software */
    SD_SPI->CR1 |= SPI_CR1_SSM;
    SPI1->CR2 &= ~SPI_CR2_NSSP;
    SPI1->CR2 &= ~SPI_CR2_SSOE;

    /* Set NSS high */
    SD_GPIO->ODR |= (1 << 4);
    /* Set MOSI high for 80 cycles (at least 74, but aligned to 8) */
    uint8_t blank[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    spi_send(blank);

    /* Send CMD0 */
    uint8_t cmd0[] = {0b01000000, 0, 0, 0, 0, 0b10010101};
    spi_send(cmd0);

    /* Revert settings */
    SD_SPI->CR1 = cr1;
    SD_SPI->CR2 = cr2;
    SD_GPIO->MODER = moder;
}

