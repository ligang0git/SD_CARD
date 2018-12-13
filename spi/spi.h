#ifndef __SPI_H
#define __SPI_H
#include <stm32l1xx.h>
#define u8 unsigned char 
#define u16 unsigned short 
#define u32 unsigned int 

#define SD_SPI1                           SPI1
#define SD_SPI1_CLK                       RCC_APB2Periph_SPI1
#define SD_SPI1_SCK_PIN                   GPIO_Pin_5                 /* PA.5 */
#define SD_SPI1_MISO_PIN                  GPIO_Pin_6                 /* PA.6 */
#define SD_SPI1_MOSI_PIN                  GPIO_Pin_7                 /* PA.7 */
#define SD_SPI1_CS_PIN                    GPIO_Pin_4                  /* PA.4 */
#define SD_SPI1_GPIO_PORT            		  GPIOA                       /* GPIOA */
#define SD_SPI1_GPIO_CLK                  RCC_AHBPeriph_GPIOA


void SD_LowLevel_DeInit(void);
void SD_LowLevel_Init(void);
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler);
u8 SPI1_ReadWriteByte(u8 TxData);



#endif

