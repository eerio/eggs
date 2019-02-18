/* SPI initialization procedure and communication abstractions
 *
 * author: Paweł Balawender
 * github.com@eerio
 */
#include<stm32f0xx.h>
#include<spi.h>

/* Send byte by SPI
 * note: it actually sends 16 bits either way, so after <data> byte
 * a null-byte will be sent. TODO: it should be clear whether its 8
 * or 16 bits communication
 *
 * CS is set by default
 */
void SPI_send(uint8_t data) {
    GPIOA->ODR &= ~(1 << 4);
    while (SPI1->SR & SPI_SR_BSY) {}
    SPI1->DR = data;
    GPIOA->ODR |= (1 << 4);
}

/* Mode: full-duplex, master
 * Pins:
 *  - PA4: NSS (Slave Select), mode: output
 *  - PA5: SCK, mode: AF0
 *  - PA6: MISO, mode: AF0
 *  - PA7: MOSI, mode: AF0
 * Procedure of SPI configuration: RM p. 765
 * SPI config:
 *  - MSB first
 *  - 8 bits per transfer
 *  - clock is low when inactive (CPOL = 0)
 *  - data is valid on clock leading edge (CPHA = 0)
 *  - enable line is active high
 */
void setup_spi(void) {
    /* Enable GPIO port A */
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    /* SCK, MISO, MOSI: Alternate Function mode
     * NSS: Output mode
     */
    GPIOA->MODER &= 0xFFFF00FF;  /* Clear GPIOA->MODER bits [15:8] */
    GPIOA->MODER |= GPIO_MODER_MODER4_0;
    GPIOA->MODER |= GPIO_MODER_MODER5_1;
    GPIOA->MODER |= GPIO_MODER_MODER6_1;
    GPIOA->MODER |= GPIO_MODER_MODER7_1;

    /* Select push-pull mode */
    GPIOA->OTYPER &= ~GPIO_OTYPER_OTYPER5;
    GPIOA->OTYPER &= ~GPIO_OTYPER_OTYPER6;
    GPIOA->OTYPER &= ~GPIO_OTYPER_OTYPER7;

    /* Select highest frequency*/
    GPIOA->OSPEEDR |= 0x0000FF00;
    
    /* Select pull-up resistors for SCK, MISO, MOSI */
    GPIOA->PUPDR &= 0xFFFF03FF;
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR5_0;
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR6_0;
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR7_0;

    /* Select Alternate Function #0 for SCK, MISO, MOSI */
    GPIOA->AFR[0] &= 0x000FFFFF;

    /* Enable SPI #1 clock */
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    /* Data mode: 2-line unidirectional */
    SPI1->CR1 &= ~SPI_CR1_BIDIMODE;
    /* Disable CRC */
    SPI1->CR1 &= ~SPI_CR1_CRCEN;
    /* Full-duplex mode */
    SPI1->CR1 &= ~SPI_CR1_RXONLY;
    /* Software slave management */
    SPI1->CR1 |= SPI_CR1_SSM;
    /* Internal slave select */
    SPI1->CR1 |= SPI_CR1_SSI;
    /* MSB first */
    SPI1->CR1 &= ~SPI_CR1_LSBFIRST;
    /* Set freq to 8Mhz / 32 = 250kHz */
    SPI1->CR1 &= SPI_CR1_BR;
    SPI1->CR1 |= (0b100) << SPI_CR1_BR_Pos;
    /* Master configuration */
    SPI1->CR1 |= SPI_CR1_MSTR;
    /* Clock to 0 when idle */
    SPI1->CR1 &= ~SPI_CR1_CPOL;
    /* First clock transition is the first data capture edge */
    SPI1->CR1 &= ~SPI_CR1_CPHA;

    /* RXNE if the FIFO level >= 8 bit */
    SPI1->CR2 |= SPI_CR2_FRXTH;
    /* Transfer data length: 8-bit */
    SPI1->CR2 &= ~SPI_CR2_DS;
    SPI1->CR2 |= 0b0111 << SPI_CR2_DS_Pos;
    /* Frame format: SPI Motorola mode */
    SPI1->CR2 &= ~SPI_CR2_FRF;
    /* Send NSS pulse between two data transfers */
    SPI1->CR2 |= SPI_CR2_NSSP;
    /* Slave Select Output disable */
    SPI1->CR2 &= ~SPI_CR2_SSOE;

    /* Enable SPI */
    SPI1->CR1 |= SPI_CR1_SPE;
}

