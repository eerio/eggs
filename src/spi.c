/* SPI initialization procedure and communication abstractions
 *
 * author: Paweł Balawender
 * github.com@eerio
 */
#include<stm32f0xx.h>
#include<spi.h>
#include<common.h>

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
 *  - hardware NSS line management
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
    SPI1->CR2 |= SPI_CR2_FRXTH;
    
    /* Hardware slave management */
    SPI1->CR1 &= ~SPI_CR1_SSM;
    /* Internal slave select (redundant with SSM bit reset) */
    // SPI1->CR1 |= SPI_CR1_SSI;
    /* Send NSS pulse between two data transfers */
    SPI1->CR2 |= SPI_CR2_NSSP;

    /* Enable SPI */
    delay(1000);
    SPI1->CR1 |= SPI_CR1_SPE;


    /* Slave Select Output enable */
    SPI1->CR2 |= SPI_CR2_SSOE;
}

