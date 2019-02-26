/* SDHC card initialization procedure and communication abstractions
 *
 * author: Paweł Balawender
 * github.com@eerio
 */
#include<sd.h>
#include<spi.h>
#include<common.h>
#define SD_SPI (SPI1)
#define SD_GPIO (GPIOA) 


uint8_t blank[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t cmd0[] = {0x40, 0x00, 0x00, 0x00, 0x00, 0x95};
uint8_t cmd8[] = {0b01001000, 0x00, 0x00, 0x01, 0b10101010, 0b00001111};
uint8_t cmd58[] = {0b01111010, 0, 0, 0, 0, 0b01110101};


void init_sd(void) {
    /* Save SPI settings */
    uint32_t moder = SD_GPIO->MODER,
             cr1 = SD_SPI->CR1,
             cr2 = SD_SPI->CR2;

    /* Change NSS management to software */
    SD_SPI->CR1 |= SPI_CR1_SSM;
    /* Select output mode for NSS pin */
    SD_GPIO->MODER &= ~GPIO_MODER_MODER4;
    SD_GPIO->MODER |= GPIO_MODER_MODER4_0;

    /* Set NSS high and set MOSI high for 96 cycles (at least 74) */
    SD_GPIO->ODR |= (1 << 4);
    spi_send(blank);
    spi_send(blank);

    /* Assert NSS and send CMD0 */
    SD_GPIO->ODR &= ~(1 << 4);
    spi_send(cmd0);
    /* TODO: Is it really needed? */
    spi_send(blank);
    spi_send(blank);
    spi_send(blank);
    /* CMD8 not supported */
    spi_send(cmd58);
    /* Keep the clock for a while */
    for (unsigned int i=0; i < 4; ++i) spi_send(blank);
    
    SD_GPIO->ODR |= (1 << 4);

    /* Revert settings */
    SD_SPI->CR1 = cr1;
    SD_SPI->CR2 = cr2;
    SD_GPIO->MODER = moder;
}

