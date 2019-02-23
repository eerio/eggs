#include<stm32f0xx.h>
#include<common.h>
#include<dma.h>

#define DMA_RX (DMA1_Channel2)
#define DMA_RX_DMA (DMA1)
#define DMA_RX_Channel (2)
#define DMA_TX (DMA1_Channel3)
#define DMA_TX_DMA (DMA1)
#define DMA_TX_Channel (3)


void configure_DMA(void) {
    /* Perform each time 1 transfer from SPI TX buffer to SPI1->DR */
    if ((DMA_TX->CCR & DMA_CCR_EN) == 0) {
        DMA_TX->CPAR = (uint32_t)(&(SPI1->DR));
        DMA_TX->CMAR = (uint32_t)(&SPI_TX_buffer);
        DMA_TX->CNDTR |= (BUFFER_SIZE << DMA_CNDTR_NDT_Pos);
    }
    /* Perform each time 1 transfer from SPI1 RX FIFO to the buffer */
    if ((DMA_RX->CCR & DMA_CCR_EN) == 0) {
        DMA_RX->CPAR = (uint32_t)(&(SPI1->DR));
        DMA_RX->CMAR = (uint32_t)(&SPI_RX_buffer);
        DMA_RX->CNDTR |= (BUFFER_SIZE << DMA_CNDTR_NDT_Pos);
    }

    /* Memory to peripheral mode */
    DMA_TX->CCR &= ~DMA_CCR_MEM2MEM;
    DMA_RX->CCR &= ~DMA_CCR_MEM2MEM;
    /* Channel priority: Very high */
    DMA_TX->CCR |= DMA_CCR_PL;
    DMA_RX->CCR |= DMA_CCR_PL;
    /* Data size in TX FIFO: 8 bits */
    DMA_TX->CCR &= ~DMA_CCR_MSIZE;
    DMA_RX->CCR &= ~DMA_CCR_MSIZE;
    /* Data size in SPI DR: 8 bits */
    DMA_TX->CCR &= ~DMA_CCR_PSIZE;
    DMA_RX->CCR &= ~DMA_CCR_PSIZE;
    /* Increment memory address */
    DMA_TX->CCR |= DMA_CCR_MINC;
    DMA_RX->CCR |= DMA_CCR_MINC;
    /* Don't increment peripheral address */
    DMA_TX->CCR &= ~DMA_CCR_PINC;
    DMA_RX->CCR &= ~DMA_CCR_PINC;
    /* No circular mode */
    DMA_TX->CCR &= ~DMA_CCR_CIRC;
    DMA_RX->CCR &= ~DMA_CCR_CIRC;
    /* Transfer direction: Memory to peripheral */
    DMA_TX->CCR |= DMA_CCR_DIR;
    DMA_RX->CCR &= ~DMA_CCR_DIR; /* Periph to mem */
}

void start_DMA(void) {
    /* Enable channels */
    DMA_TX->CCR |= DMA_CCR_EN;
    DMA_RX->CCR |= DMA_CCR_EN;
}

void disable_dma(void) {
    /* Disable channels */
    DMA_TX->CCR &= ~DMA_CCR_EN;
    DMA_RX->CCR &= ~DMA_CCR_EN;
}

