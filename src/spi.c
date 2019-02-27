/* SPI initialization procedure and communication abstractions
 *
 * author: Paweł Balawender
 * github.com@eerio
 */
#include<stm32f0xx.h>
#include<common.h>
#include<spi.h>
#include<dma.h>

void spi_send(uint8_t* x) {
    SPI1->CR2 |= SPI_CR2_RXDMAEN;
    for(unsigned int i=0; i < TX_BUFFER_SIZE; ++i) {
        SPI_TX_buffer[i] = x[i];
    }
    SPI1->CR2 |= SPI_CR2_TXDMAEN;
    while (SPI1->CR2 & SPI_CR2_TXDMAEN) {}
    while (SPI1->SR & SPI_SR_BSY) {}

    //while (SPI1->CR2 & SPI_CR2_RXDMAEN) {}
}

/* Mode: full-duplex, master
 * Pins (all in AF0 mode):
 *  - PA4: NSS (Slave Select)
 *  - PA5: SCK
 *  - PA6: MISO
 *  - PA7: MOSI
 * Procedure of SPI configuration: RM p. 765
 * SPI config:
 *  - MSB first
 *  - 8 bits per transfer
 *  - clock is low when inactive (CPOL = 0)
 *  - data is valid on clock leading edge (CPHA = 0)
 *  - hardware NSS line management with NSSP enabled
 */
void setup_spi(void) {
    /* Enable GPIO port A */
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    /* Enable SPI #1 clock */
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    
    /* Set AF0 mode for every pin */
    GPIOA->MODER &= 0xFFFF00FF;
    GPIOA->MODER |= GPIO_MODER_MODER4_1;
    GPIOA->MODER |= GPIO_MODER_MODER5_1;
    GPIOA->MODER |= GPIO_MODER_MODER6_1;
    GPIOA->MODER |= GPIO_MODER_MODER7_1;
    /* Select Alternate Function #0 for SCK, MISO, MOSI */
    GPIOA->AFR[0] &= 0x0000FFFF;

    /* Select push-pull mode */
    GPIOA->OTYPER &= 0xFFFFFF0F;
    /* Select pull-up resistors for NSS, SCK, MISO, MOSI */
    GPIOA->PUPDR &= 0xFFFF00FF;
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR4_0;
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR5_0;
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR6_0;
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR7_0;
    /* Select highest frequency*/
    GPIOA->OSPEEDR |= 0x0000FF00;


    /* Data mode: 2-line unidirectional */
    SPI1->CR1 &= ~SPI_CR1_BIDIMODE;
    /* Full-duplex mode */
    SPI1->CR1 &= ~SPI_CR1_RXONLY;
    /* Set freq to 8Mhz / 32 = 250kHz */
    SPI1->CR1 &= SPI_CR1_BR;
    SPI1->CR1 |= (0b100) << SPI_CR1_BR_Pos;
    /* Master configuration */
    SPI1->CR1 |= SPI_CR1_MSTR;
    /* Clock to 0 when idle */
    SPI1->CR1 &= ~SPI_CR1_CPOL;
    /* First clock transition is the first data capture edge */
    SPI1->CR1 &= ~SPI_CR1_CPHA;
    
    /* MSB first */
    SPI1->CR1 &= ~SPI_CR1_LSBFIRST;
    /* Frame format: SPI Motorola mode */
    SPI1->CR2 &= ~SPI_CR2_FRF;
    /* Transfer data length: 8-bit */
    SPI1->CR2 &= ~SPI_CR2_DS;
    SPI1->CR2 |= 0b0111 << SPI_CR2_DS_Pos;
    
    /* Disable CRC */
    SPI1->CR1 &= ~SPI_CR1_CRCEN;
    /* RXNE if the FIFO level >= 8 bit */
    // SPI1->CR2 |= SPI_CR2_FRXTH;
    
    /* Hardware slave management */
    SPI1->CR1 &= ~SPI_CR1_SSM;
    /* Send NSS pulse between two data transfers */
    SPI1->CR2 |= SPI_CR2_NSSP;
    /* Slave Select Output enable */
    SPI1->CR2 |= SPI_CR2_SSOE;

    /* Enable DMA. Procedure: p. 770 */
    // SPI1->CR2 |= SPI_CR2_RXDMAEN;
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    DMA1->CSELR &= 0xFFFFF00F;
    DMA1->CSELR |= (0b0011 << 4);
    DMA1->CSELR |= (0b0011 << 8);
    configure_DMA(); 
    // SPI1->CR2 |= SPI_CR2_TXDMAEN;

    /* Enable SPI */
    /* At this point MOSI line and clock are being pulled down */
    SPI1->CR1 |= SPI_CR1_SPE;

    /* Start DMA */
    start_DMA();
}


/* procedure: p. 768, 770 */
void disable_spi(void) {
    /* Disable dma */
    disable_dma();
    
    /* Wait until FTLVL[1:] == 0 (no more data to transmit) */
    while (SPI1->SR & SPI_SR_FTLVL) {}
    /* Wait until BSY = 0 (the last data frame is processed) */
    while (SPI1->SR & SPI_SR_BSY) {}
    /* Disable SPI */
    SPI1->CR1 &= ~SPI_CR1_SPE;

    /* Read data until FRLVL[1:0] == 00 (read all the received data) */
    while(SPI1->SR & SPI_SR_FRLVL) {}

    /* Disable DMA TX and RX buffers in SPI */
    SPI1->CR2 &= ~(SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN);
}

