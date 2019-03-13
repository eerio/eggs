#define USE_DEFAULT_SPI_SD_TX_DMA
#define USE_DEFAULT_SPI_SD_RX_DMA

void configure_DMA(uint8_t[], uint8_t[], uint16_t, uint16_t);
void start_DMA(void);
void disable_DMA(void);


