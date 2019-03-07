
#define DMA_SPI_RX (DMA1_Channel2)
#define DMA_SPI_RX_DMA (DMA1)
#define DMA_SPI_RX_Channel (2U)
#define DMA_SPI_TX (DMA1_Channel3)
#define DMA_SPI_TX_DMA (DMA1)
#define DMA_SPI_TX_Channel (3U)

#define USE_DEFAULT_SPI_SD_TX_DMA
#define USE_DEFAULT_SPI_SD_RX_DMA

void configure_DMA(void);
void start_DMA(void);
void disable_DMA(void);


