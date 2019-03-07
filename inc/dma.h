#include<stm32f0xx.h>

#define DMA_SPI_RX DMA1_Channel2
#define DMA_SPI_TX DMA1_Channel3
#define DMA_SPI_RX_DMA (DMA1)
#define DMA_SPI_TX_DMA (DMA1)
#define DMA_SPI_RX_Channel (2)
#define DMA_SPI_TX_Channel (3)


void configure_DMA(void);
void start_DMA(void);
void disable_dma(void);

/* According to p. 198 RefMan, DMA can be mapped to:
 * SPI1_RX: DMA1_Channel2 or DMA2_Channel3
 * SPI1_TX: DMA1_Channel3 or DMA2_Channel4
 * SPI2_RX: DMA1_Channel4 or DMA1_Channel6
 * SPI2_TX: DMA1_Channel5 or DMA1_Channel7
 */
#if DMA_SPI_RX == DMA1_Channel2 \
 || DMA_SPI_TX == DMA1_Channel3

void DMA1_Ch2_3_DMA2_Ch1_2_IRQHandler(void);
#endif


#if DMA_SPI_RX == DMA2_Channel3 \
 || DMA_SPI_RX == DMA1_Channel4 \
 || DMA_SPI_RX == DMA2_Channel6 \
 || DMA_SPI_TX == DMA2_Channel4 \
 || DMA_SPI_TX == DMA1_Channel5 \
 || DMA_SPI_TX == DMA1_Channel7

void DMA1_Ch4_7_DMA2_Ch3_5_IRQHandler(void);
#endif

