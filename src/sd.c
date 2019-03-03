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
uint8_t cmd8[] = {0b01001000, 0x00, 0x00, 0x01, 0xAA, 0b00001111};
uint8_t cmd58[] = {0b01111010, 0, 0, 0, 0, 0b01110101};
uint8_t cmd55[] = {0b01110111, 0, 0, 0, 0, 1};
uint8_t acmd41[] = {0b01101001, 0, 0, 0, 0, 1};


void init_sd(void) {
    volatile uint8_t *resp;

    /* Toggle CLK for 96 cycles (at least 74) */
    spi_send(blank);
    spi_send(blank);

    /* Spam CMD0 until there's a response */
    do {
        spi_send(cmd0);
        spi_send(blank);
        resp = spi_read();
    } while (*resp == 0xFF);

    /* Send CMD8, check if Illegal Command in responce */
    spi_send(cmd8);
    spi_send(blank);
    resp = spi_read();

    if (*resp & (1 << 2)) {
        /* Illegal command -> Ver 1.X SD card or not a SD card */
    } else {
        /* No Illegal command in reponce -> Ver >= 2.0 SD card
         * Check if CRC error or non-compatible voltage range
         */


    }

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
}

