
#define DMA_SPI_RX (DMA1_Channel2)
#define DMA_SPI_RX_DMA (DMA1)
#define DMA_SPI_RX_Channel (2U)
#define DMA_SPI_TX (DMA1_Channel3)
#define DMA_SPI_TX_DMA (DMA1)
#define DMA_SPI_TX_Channel (3U)

#define USE_DEFAULT_SPI_SD_TX_DMA
#define USE_DEFAULT_SPI_SD_RX_DMA

void configure_DMA(uint8_t[], uint8_t[], uint16_t, uint16_t);
void start_DMA(void);
void disable_DMA(void);


