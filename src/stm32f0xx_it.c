#include<stm32f0xx.h>

#ifdef USE_DEFAULT_SPI_SD_TX_DMA
void DMA1_Ch2_3_DMA2_Ch1_2_IRQHandler(void) {
    SPI1->CR2 &= ~SPI_CR2_TXDMAEN;
    DMA1->IFCR |= DMA_IFCR_CTCIF3;
}
#else
void DMA1_Ch2_3_DMA2_Ch1_2_IRQHandler(void) {
    SPI1->CR2 &= ~SPI_CR2_TXDMAEN;
    DMA1->IFCR |= DMA_IFCR_CTCIF3;
}
#endif

