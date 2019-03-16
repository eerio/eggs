#include<stm32f0xx.h>
#include<sys.h>

void DMA1_Ch2_3_DMA2_Ch1_2_IRQHandler(void) {
    if (DMA_SPI_TX_DMA->ISR & DMA_ISR_TCIF3) {
        SPI1->CR2 &= ~SPI_CR2_TXDMAEN;
        DMA1->IFCR |= DMA_IFCR_CTCIF3;
    } else if (DMA_SPI_RX_DMA->ISR & DMA_ISR_TCIF2) {
        DMA1->IFCR |= DMA_IFCR_CTCIF2;
    }
}

