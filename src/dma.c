#include<stm32f0xx.h>
#include<common.h>
#include<dma.h>

void configure_DMA(void) {
    //setup_DMA_RX();
    setup_DMA_TX();
}

/* Transfer data from SPI1 TX container in memory to SPI1 TX FIFO */
void setup_DMA_TX(void) {
    /* Perform each time 1 transfer from SPI TX buffer to SPI1->DR */
    if ((DMA_TX->CCR & DMA_CCR_EN) == 0) {
        //DMA_TX->CPAR = (uint32_t)(&(SPI1->DR));
        DMA_TX->CPAR = (uint32_t)(&SPI_RX_buffer);
        DMA_TX->CMAR = (uint32_t)(&SPI_TX_buffer);
        DMA_TX->CNDTR |= (1U << DMA_CNDTR_NDT_Pos);
    }

    /* Memory to peripheral mode */
    //DMA_TX->CCR &= ~DMA_CCR_MEM2MEM;
    DMA_TX->CCR |= DMA_CCR_MEM2MEM;
    /* Channel priority: Very high */
    DMA_TX->CCR |= DMA_CCR_PL;
    /* Data size in TX FIFO: 8 bits */
    DMA_TX->CCR &= ~DMA_CCR_MSIZE;
    /* Data size in SPI DR: 8 bits */
    DMA_TX->CCR &= ~DMA_CCR_PSIZE;
    //DMA_TX->CCR |= DMA_CCR_PSIZE_0;
    /* Increment memory address */
    DMA_TX->CCR |= DMA_CCR_MINC;
    /* Don't increment peripheral address */
    //DMA_TX->CCR &= ~DMA_CCR_PINC;
    DMA_TX->CCR |= DMA_CCR_PINC;
    /* No circular mode */
    DMA_TX->CCR &= ~DMA_CCR_CIRC;
    /* Transfer direction: Memory to peripheral */
    DMA_TX->CCR |= DMA_CCR_DIR;

    /* Enable channel */
    DMA_TX->CCR |= DMA_CCR_EN;
}


/* Transfer data from SPI1 RX FIFO to SPI RX buffer in memory */
void setup_DMA_RX(void) {
    /* Perform each time 1 transfer from SPI1 RX FIFO to the buffer */
    if ((DMA_RX->CCR & DMA_CCR_EN) == 0) {
        DMA_RX->CPAR = (uint32_t)(&(SPI1->DR));
        DMA_RX->CMAR = (uint32_t)(&SPI_RX_buffer);
        DMA_RX->CNDTR |= (1U << DMA_CNDTR_NDT_Pos);
    }

    /* Peripheral to memory mode */
    DMA_RX->CCR &= ~DMA_CCR_MEM2MEM;
    /* Channel priority: Very high */
    DMA_RX->CCR |= DMA_CCR_PL;
    /* Data size in RX buffer: 8 bits */
    DMA_RX->CCR &= ~DMA_CCR_MSIZE;
    /* Data size in SPI DR: 8 bits */
    DMA_RX->CCR &= ~DMA_CCR_PSIZE;
    /* Increment memory address */
    DMA_RX->CCR |= DMA_CCR_MINC;
    /* Don't increment peripheral address */
    DMA_RX->CCR &= ~DMA_CCR_PINC;
    /* Circular mode */
    DMA_RX->CCR &= ~DMA_CCR_CIRC;
    /* Transfer direction: Peripheral to memory*/
    DMA_RX->CCR &= ~DMA_CCR_DIR;

    /* Enable channel */
    DMA_RX->CCR |= DMA_CCR_EN;
}

