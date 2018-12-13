#include "spi.h"

void SD_LowLevel_DeInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	SPI_Cmd(SD_SPI1, DISABLE); /*!< SD_SPI disable */
	SPI_DeInit(SD_SPI1);   /*!< DeInitializes the SD_SPI */
	
	RCC_APB2PeriphClockCmd(SD_SPI1_CLK, DISABLE); //时钟失能
	//配置spi引脚
	GPIO_InitStructure.GPIO_Pin = SD_SPI1_SCK_PIN|SD_SPI1_MISO_PIN|SD_SPI1_MOSI_PIN|SD_SPI1_CS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(SD_SPI1_GPIO_PORT, &GPIO_InitStructure);
}

void SD_LowLevel_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef   SPI_InitStructure;
	//GPIO和SPI时钟使能
	RCC_AHBPeriphClockCmd(SD_SPI1_GPIO_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(SD_SPI1_CLK, ENABLE); 
	//配置SPI引脚
	GPIO_InitStructure.GPIO_Pin = SD_SPI1_SCK_PIN|SD_SPI1_MISO_PIN|SD_SPI1_MOSI_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(SD_SPI1_GPIO_PORT, &GPIO_InitStructure);
	//CS引脚
	GPIO_InitStructure.GPIO_Pin = SD_SPI1_CS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_Init(SD_SPI1_GPIO_PORT, &GPIO_InitStructure);
	//配置备用功能
   /* Connect PXx to SD_SPI_SCK */
   GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
 
   /* Connect PXx to SD_SPI_MISO */
   GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1); 
 
   /* Connect PXx to SD_SPI_MOSI */
   GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);	
	
	/*!< SD_SPI Config */
   SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
   SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
   SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
   SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
   SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
   SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
   SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
 
   SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
   SPI_InitStructure.SPI_CRCPolynomial = 7;
   SPI_Init(SD_SPI1, &SPI_InitStructure);
   
   SPI_Cmd(SD_SPI1, ENABLE); /*!< SD_SPI enable */
}

//SPI 速度设置函数
//SpeedSet:
//SPI_BaudRatePrescaler_2   2分频   (SPI 36M@sys 72M)
//SPI_BaudRatePrescaler_8   8分频   (SPI 9M@sys 72M)
//SPI_BaudRatePrescaler_16  16分频  (SPI 4.5M@sys 72M)
//SPI_BaudRatePrescaler_256 256分频 (SPI 281.25K@sys 72M) 
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI1->CR1&=0XFFC7;
	SPI1->CR1|=SPI_BaudRatePrescaler;	//设置SPI2速度 
	SPI_Cmd(SPI1,ENABLE); 

} 


//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI1_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
		{
		retry++;
		if(retry>200)return 0;
		}			  
	SPI_I2S_SendData(SPI1, TxData); //通过外设SPIx发送一个数据

	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据					    
}

