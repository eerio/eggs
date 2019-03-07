#include<stm32f0xx.h>

void DMA1_Ch2_3_DMA2_Ch1_2_IRQHandler(void) {
    SPI1->CR2 &= ~SPI_CR2_TXDMAEN;
    DMA1->IFCR |= DMA_IFCR_CTCIF3;
}

