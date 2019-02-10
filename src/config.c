/* System configuration procedures
 * note: they're designed specifically for STM32F091RC development board
 *
 * author: Paweł Balawender
 * github.com@eerio
 */
#include<stm32f0xx.h>
#include<config.h>


void init_sys(void) {
    /* init_leds(); */
    setup_spi();
}

/* Mode: full-duplex, master
 * Pins:
 *  - PA4: NSS (Slave Select)
 *  - PA5: SCK
 *  - PA6: MISO
 *  - PA7: MOSI
 * Procedure of SPI configuration: RM p. 765
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
    GPIOA->OTYPER &= 0xFFFFFF0F;

    /* Select highest frequency*/
    GPIOA->OSPEEDR |= 0x0000FF00;
    
    /* Select pull-up resistors for SCK, MISO, MOSI */
    //GPIOA->PUPDR &= 0xFFFF00FF;
    /* pyll-down for clock */
    //GPIOA->PUPDR |= 2 << 10;
    //GPIOA->PUPDR |= GPIO_PUPDR_PUPDR5_0;
    //GPIOA->PUPDR |= GPIO_PUPDR_PUPDR6_0;
    //GPIOA->PUPDR |= GPIO_PUPDR_PUPDR7_0;

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
    /* Set baudrate to PCLK / 32 */
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

/* Enable GPIOA and set the correct pin modes for PA5, PA6 and PA7 */
void init_leds(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIOA->MODER |= GPIO_MODER_MODER5_0;
    GPIOA->MODER |= GPIO_MODER_MODER6_0;
    GPIOA->MODER |= GPIO_MODER_MODER7_0;
}

