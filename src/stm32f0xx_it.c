#include<stm32f0xx.h>

/* it should be able to make 2 handlers. it creates 1 only always */
#if defined(USE_DEFAULT_SPI_SD_TX_DMA) || defined(USE_DEFAULT_SPI_SD_RX_DMA)
void DMA1_Ch2_3_DMA2_Ch1_2_IRQHandler(void) {
    if (DMA_TX->ISR & DMA_ISR_TCIF3) {
        SPI1->CR2 &= ~SPI_CR2_TXDMAEN;
        DMA1->IFCR |= DMA_IFCR_CTCIF3;
    } else if (DMA_RX->ISR & DMA_ISR_TCIF2) {
        SPI_RX_ind = 0;
        DMA1->IFCR |= DMA_IFCR_CTCIF2;
    }
}
#else
void DMA1_Ch2_3_DMA2_Ch1_2_IRQHandler(void) {
    SPI1->CR2 &= ~SPI_CR2_TXDMAEN;
    DMA1->IFCR |= DMA_IFCR_CTCIF3;
}
#endif

