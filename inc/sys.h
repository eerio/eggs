
#include<stm32f0xx.h>
#include<config.h>

#ifdef SD_USE_SPI1
#define SPI_SD (SPI1)
#define RCC_SPI_SD_EN (RCC_APB2ENR_SPI1EN)
#else
#define SPI_SD (SPI2)
#define RCC_SPI_SD_EN (RCC_APB2ENR_SPI2EN)
#endif

