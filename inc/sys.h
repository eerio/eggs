
#include<stm32f0xx.h>
#include<config.h>

#ifdef SD_USE_SPI1
#define SPI_SD (SPI1)
#define RCC_SPI_SD_EN (RCC_APB2ENR_SPI1EN)
#else
#define SPI_SD (SPI2)
#define RCC_SPI_SD_EN (RCC_APB2ENR_SPI2EN)
#endif

#define DMA_SPI_RX (DMA1_Channel2)
#define DMA_SPI_RX_DMA (DMA1)
#define DMA_SPI_RX_Channel (2U)
#define DMA_SPI_TX (DMA1_Channel3)
#define DMA_SPI_TX_DMA (DMA1)
#define DMA_SPI_TX_Channel (3U)

#define SD_USE_SPI1


/* Initialize board's peripherals */
void init_sys(void);

void quit_sys(void);

