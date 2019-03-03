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
uint8_t cmd8[] = {0b01001000, 0x00, 0x00, 0x01, 0b10101010,1}; //0b00001111};
uint8_t cmd58[] = {0b01111010, 0, 0, 0, 0, 1};//0b01110101};
uint8_t cmd55[] = {0b01110111, 0, 0, 0, 0, 1};
uint8_t acmd41[] = {0b01101001, 0, 0, 0, 0, 1};


void init_sd(void) {
    /* Save SPI settings */
    uint32_t moder = SD_GPIO->MODER,
             cr1 = SD_SPI->CR1,
             cr2 = SD_SPI->CR2;
    volatile uint8_t *resp;

    /* Change NSS management to software */
    //SD_SPI->CR1 |= SPI_CR1_SSM;
    /* Select output mode for NSS pin */
    //SD_GPIO->MODER &= ~GPIO_MODER_MODER4;
    //SD_GPIO->MODER |= GPIO_MODER_MODER4_0;

    /* Set NSS high and set MOSI high for 96 cycles (at least 74) */
    //SD_GPIO->ODR |= (1 << 4);
    spi_send(blank);
    spi_send(blank);

    /* Assert NSS to send commands */
    //SD_GPIO->ODR &= ~(1 << 4);

    /* Spam CMD0 */
    do {
        spi_send(cmd0);
        spi_send(blank);
        resp = spi_read();
    } while (*resp == 0xFF);

    /* Read SD card's OCR register and give clock for at least 6+1 bytes */
    spi_send(cmd58);
    spi_send(blank);
    spi_send(blank);
    resp = spi_read() - 6;

    /* Assert no errors in R1 */
    if (resp[0] != 0x01) while (1) {
        /* Not an SD card (invalid response for CMD58 command) */
    }
    /* Assert voltage OK  (bit 20: 3.2-3.3V, bit 32: 3.3-3.4V) */
    /* TODO: it should only check 3.3V bits! */
    if ((resp[2] != 0xFF) || (resp[3] != 0x80)) while (1) {
        /* Invalid operating voltage range */
    }

    /* Spam ACMD41 until SD card's not busy (so it's ready)
     * preceed ACMD41 with CMD55 to inform SD that the next
     * command is special; ACMD41 starts SD initialization process
     *
     * ref: p.227 PhysicalLayerSimplifiedSpecification, p.7 lec12
     */
    spi_send(cmd55);
    spi_send(acmd41);
    spi_send(blank);

    /* Loop until card's power up procedure is finished
     * ref: SD Product Manual, p. 37
     */
    do {
        spi_send(cmd58);
        spi_send(blank);
        spi_send(blank);
        resp = spi_read() - 6;
    } while ((resp[1] & (1 << 7))== 0);

    /* Revert settings */
    //SD_GPIO->ODR |= (1 << 4);
    SD_SPI->CR1 = cr1;
    SD_SPI->CR2 = cr2;
    SD_GPIO->MODER = moder;
}

