#include<stm32f0xx.h>
#include<common.h> /* buffers */
#include<dma.h>
#include<spi.h> /* SPI_SD */

void configure_DMA(void) {
    /* Peripheral: SD's SPI */
    DMA_SPI_TX->CPAR = (uint32_t)(&(SPI_SD->DR));
    DMA_SPI_RX->CPAR = (uint32_t)(&(SPI_SD->DR));
    /* Memory address: buffers */
    DMA_SPI_TX->CMAR = (uint32_t)(&SPI_TX_buffer);
    DMA_SPI_RX->CMAR = (uint32_t)(&SPI_RX_buffer);
    /* Not to cause a buffer overflow, set CNDTR to buffers' sizes */
    DMA_SPI_TX->CNDTR |= (SPI_TX_BUFFER_SIZE << DMA_CNDTR_NDT_Pos);
    DMA_SPI_RX->CNDTR |= (SPI_RX_BUFFER_SIZE << DMA_CNDTR_NDT_Pos);
    /* Memory to peripheral mode */
    DMA_SPI_TX->CCR &= ~DMA_CCR_MEM2MEM;
    DMA_SPI_RX->CCR &= ~DMA_CCR_MEM2MEM;
    /* Channel priority: Very high */
    DMA_SPI_TX->CCR |= DMA_CCR_PL;
    DMA_SPI_RX->CCR |= DMA_CCR_PL;
    /* Data size in TX FIFO: 8 bits */
    DMA_SPI_TX->CCR &= ~DMA_CCR_MSIZE;
    DMA_SPI_RX->CCR &= ~DMA_CCR_MSIZE;
    /* Data size in SPI DR: 8 bits */
    DMA_SPI_TX->CCR &= ~DMA_CCR_PSIZE;
    DMA_SPI_RX->CCR &= ~DMA_CCR_PSIZE;
    /* Increment memory address */
    DMA_SPI_TX->CCR |= DMA_CCR_MINC;
    DMA_SPI_RX->CCR |= DMA_CCR_MINC;
    /* Don't increment peripheral address */
    DMA_SPI_TX->CCR &= ~DMA_CCR_PINC;
    DMA_SPI_RX->CCR &= ~DMA_CCR_PINC;
    /* Circular mode */
    DMA_SPI_TX->CCR |= DMA_CCR_CIRC;
    DMA_SPI_RX->CCR |= DMA_CCR_CIRC;
    /* Transfer direction */
    DMA_SPI_TX->CCR |= DMA_CCR_DIR; /* Memory to peripheral */
    DMA_SPI_RX->CCR &= ~DMA_CCR_DIR; /* Peripheral to memory */

    /* Transfer complete interrupt enable */
    DMA_SPI_TX->CCR |= DMA_CCR_TCIE;
#ifdef USE_DEFAULT_SPI_SD_TX_DMA
    NVIC_EnableIRQ(DMA1_Ch2_3_DMA2_Ch1_2_IRQn);
    NVIC_SetPriority(DMA1_Ch2_3_DMA2_Ch1_2_IRQn, 0);
#else
    NVIC_EnableIRQ(DMA1_Ch4_7_DMA2_Ch3_5_IRQn);
    NVIC_SetPriority(DMA1_Ch4_7_DMA2_Ch3_5_IRQn, 0);
#endif
}

void start_DMA(void) {
    /* Enable channels */
    DMA_SPI_TX->CCR |= DMA_CCR_EN;
    DMA_SPI_RX->CCR |= DMA_CCR_EN;
}

void disable_DMA(void) {
    /* Disable channels */
    while(DMA_SPI_TX->CNDTR != SPI_TX_BUFFER_SIZE) {}
    DMA_SPI_TX->CCR &= ~DMA_CCR_EN;
    DMA_SPI_RX->CCR &= ~DMA_CCR_EN;
}

