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
    /* Perform each time 6 transfers from SPI TX buffer to SPI1->DR */
    if ((DMA_TX->CCR & DMA_CCR_EN) == 0) {
        DMA_TX->CPAR = (uint32_t)(&(SPI1->DR));
        DMA_TX->CMAR = (uint32_t)(&SPI_TX_buffer);
        DMA_TX->CNDTR |= (TX_BUFFER_SIZE << DMA_CNDTR_NDT_Pos);
    }
    /* Perform each time 1 transfer from SPI1 RX FIFO to the buffer */
    if ((DMA_RX->CCR & DMA_CCR_EN) == 0) {
        DMA_RX->CPAR = (uint32_t)(&(SPI1->DR));
        DMA_RX->CMAR = (uint32_t)(&SPI_RX_buffer);
        DMA_RX->CNDTR |= (RX_BUFFER_SIZE << DMA_CNDTR_NDT_Pos);
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
    /* Circular mode */
    DMA_TX->CCR |= DMA_CCR_CIRC;
    DMA_RX->CCR |= DMA_CCR_CIRC;
    /* Transfer direction: Memory to peripheral */
    DMA_TX->CCR |= DMA_CCR_DIR;
    DMA_RX->CCR &= ~DMA_CCR_DIR; /* Periph to mem */

    /* Transfer complete interrupt enable */
    DMA_TX->CCR |= DMA_CCR_TCIE;
    //DMA_RX->CCR |= DMA_CCR_TCIE;
    NVIC_EnableIRQ(DMA1_Ch2_3_DMA2_Ch1_2_IRQn);
    NVIC_SetPriority(DMA1_Ch2_3_DMA2_Ch1_2_IRQn, 0);
}

void DMA1_Ch2_3_DMA2_Ch1_2_IRQHandler(void) {
    if (DMA1->ISR & DMA_ISR_TCIF3) {
        SPI1->CR2 &= ~SPI_CR2_TXDMAEN;
        DMA1->IFCR |= DMA_IFCR_CTCIF3;
    }
}

void start_DMA(void) {
    /* Enable channels */
    DMA_TX->CCR |= DMA_CCR_EN;
    DMA_RX->CCR |= DMA_CCR_EN;
}

void disable_dma(void) {
    /* Disable channels */
    while(DMA_TX->CNDTR != 6) {}
    DMA_TX->CCR &= ~DMA_CCR_EN;
    DMA_RX->CCR &= ~DMA_CCR_EN;
}

