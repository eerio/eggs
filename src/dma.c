#include<stm32f0xx.h>
#include<common.h>
#include<dma.h>

void configure_DMA(void) {
    /* Perform each time 1 transfer from SPI TX buffer to SPI1->DR */
    if ((DMA_SPI_TX->CCR & DMA_CCR_EN) == 0) {
        DMA_SPI_TX->CPAR = (uint32_t)(&(SPI1->DR));
        DMA_SPI_TX->CMAR = (uint32_t)(&SPI_TX_buffer);
        DMA_SPI_TX->CNDTR |= (SPI_TX_BUFFER_SIZE << DMA_CNDTR_NDT_Pos);
    }
    /* Perform each time 1 transfer from SPI1 RX FIFO to the buffer */
    if ((DMA_SPI_RX->CCR & DMA_CCR_EN) == 0) {
        DMA_SPI_RX->CPAR = (uint32_t)(&(SPI1->DR));
        DMA_SPI_RX->CMAR = (uint32_t)(&SPI_RX_buffer);
        DMA_SPI_RX->CNDTR |= (SPI_RX_BUFFER_SIZE << DMA_CNDTR_NDT_Pos);
    }

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
    /* No circular mode */
    // DMA_SPI_TX->CCR &= ~DMA_CCR_CIRC;
    // DMA_SPI_RX->CCR &= ~DMA_CCR_CIRC;
    /* Circular mode */
    DMA_SPI_TX->CCR |= DMA_CCR_CIRC;
    DMA_SPI_RX->CCR |= DMA_CCR_CIRC;
    /* Transfer direction: Memory to peripheral */
    DMA_SPI_TX->CCR |= DMA_CCR_DIR;
    DMA_SPI_RX->CCR &= ~DMA_CCR_DIR; /* Periph to mem */

    /* Transfer complete interrupt enable */
    DMA_SPI_TX->CCR |= DMA_CCR_TCIE;
    NVIC_EnableIRQ(DMA1_Ch2_3_DMA2_Ch1_2_IRQn);
    NVIC_SetPriority(DMA1_Ch2_3_DMA2_Ch1_2_IRQn, 0);
}

void DMA1_Ch2_3_DMA2_Ch1_2_IRQHandler(void) {
    SPI1->CR2 &= ~SPI_CR2_TXDMAEN;
    DMA1->IFCR |= DMA_IFCR_CTCIF3;
}

void start_DMA(void) {
    /* Enable channels */
    DMA_SPI_TX->CCR |= DMA_CCR_EN;
    DMA_SPI_RX->CCR |= DMA_CCR_EN;
}

void disable_dma(void) {
    /* Disable channels */
    while(DMA_SPI_TX->CNDTR != 6) {}
    DMA_SPI_TX->CCR &= ~DMA_CCR_EN;
    DMA_SPI_RX->CCR &= ~DMA_CCR_EN;
}

