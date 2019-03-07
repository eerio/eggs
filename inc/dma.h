
#define DMA_SPI_RX (DMA1_Channel2)
#define DMA_SPI_RX_DMA (DMA1)
#define DMA_SPI_RX_Channel (2U)
#define DMA_SPI_TX (DMA1_Channel3)
#define DMA_SPI_TX_DMA (DMA1)
#define DMA_SPI_TX_Channel (3U)


void configure_DMA(void);
void start_DMA(void);
void DMA1_Ch2_3_DMA2_Ch1_2_IRQHandler(void);
void disable_dma(void);

