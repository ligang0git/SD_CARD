#include "main.h"
#include "usart.h"
#include "led.h"
#include "stdlib.h"
#include "string.h"
#include "delay.h"
#include "spi.h"
#include "sd.h"
#include "ff.h"
#include "myfatfs.h"

/**
  ******************************************************************************
  *                            变量扩展声明
  ******************************************************************************
  */
extern FATFS fs;													/* FatFs文件系统对象 */
extern FIL fnew;													/* 文件对象 */
extern FRESULT res_sd;                /* 文件操作结果 */
extern UINT fnum;            					  /* 文件成功读写数量 */
extern BYTE WriteBuffer[];
extern BYTE ReadBuffer[512];


int main(void)
{		 
//	uint32_t sd_size;
//	u8 t=0;	 
	
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	delay_init();	    	 //延时函数初始化	  
	uart_init(115200);	 	//串口初始化为9600
	LED_Init();		  		//初始化与LED连接的硬件接口
	LED_D1_1;
	LED_D2_1;
	
		printf("\r\n****** 这是一个SD卡 文件系统实验 ******\r\n");
	//在外部SD卡挂载文件系统，文件系统挂载时会对SDIO设备初始化
	
	SD_FAT_Initialize("0:");
	SD_FAT_DeleteFile("TEMP1.CSV");

	SD_FAT_WriteInitialize("0:TEMP1.CSV","ID.Noumber,Year/Month/Day Hour:Minute:Second,Temp1,Temp2,Temp3,Temp4,Temp5\n");
	SD_FAT_WriteExisiting("0:TEMP1.CSV",WriteBuffer);
	SD_FAT_Read("0:TEMP1.CSV");

	
	SD_FAT_DISMOUNT("0:");
	
  
  
	while(1)
	{
	}    
	
}




#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

