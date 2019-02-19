#define DMA_RX (DMA1_Channel2)
#define DMA_RX_DMA (DMA1)
#define DMA_RX_Channel (2)
#define DMA_TX (DMA1_Channel3)
#define DMA_TX_DMA (DMA1)
#define DMA_TX_Channel (3)

void configure_DMA(void);
void setup_DMA_RX(void);
void setup_DMA_TX(void);
