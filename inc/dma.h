/**
 * @file dma.h
 * @brief DMA module interface for SPI peripheral used by the SD card driver
 * @author Pawel Balawender
 * @version Alpha
 * @date 2019-03-15
 */
#include<config.h> /* SD_USE_DEFAULT_DMA_[TX / RX] */

 /**
  * @brief Configure DMA channels for SD driver's SPI peripheral's RX and TX 
  *
  * @param tx_buffer[] Pointer to the buffer of SPI transmitter
  * @param rx_buffer[] Pointer to the buffer of SPI receiver
  * @param tx_buffer_size 
  * @param rx_buffer_size
  */
void dma_config(
        const volatile uint8_t tx_buffer[],
        const volatile uint8_t rx_buffer[],
        uint16_t tx_buffer_size,
        uint16_t rx_buffer_size
        );

/**
 * @brief 
 */
void dma_enable(void);

/**
 * @brief Safely disable DMA channels of both SPI transmitter and receiver
 */
void dma_disable(void);

