#include "myfatfs.h"
#include "string.h"
/**********************************
				文件系统变量定义
**********************************/
FATFS fs;													/* FatFs文件系统对象 */
FIL fnew;													/* 文件对象 */
FRESULT res_sd;                /* 文件操作结果 */
UINT fnum;            					  /* 文件成功读写数量 */
FILINFO* filinfo;
/**********************************
					数据缓冲区
**********************************/
BYTE ReadBuffer[512]={0};        /* 读缓冲区 */
BYTE WriteBuffer[] =              /* 写缓冲区*/
{"ID01,2018/12/03 11:00,25,27,26,31,30,\nID02,2018/12/03 11:21,24,44,32,31,25,\nID03,2018/12/03 11:35,53,34,54,23,54,\nID04,2018/12/03 11:55,33,42,15,34,23\n"
};  



/***********文件系统操作***********************************************/

/**********************************
文件系统初始化函数
功能：进行SD卡挂载，检测sd是否存在文件系统；
输入参数：path    (Logical drive number to be mounted/unmounted )
返回值：无；
示例："0:"
**********************************/
void SD_FAT_Initialize(
	const TCHAR* path	/* Logical drive number to be mounted/unmounted */
)
{
	res_sd = f_mount(&fs,path,1);  //挂载SD卡
	/*----------------------- 格式化测试 ---------------------------*/  
		/* 如果没有文件系统就格式化创建创建文件系统 */
		if(res_sd == FR_NO_FILESYSTEM)
		{
			Uart1_SendStr("》SD卡还没有文件系统，即将进行格式化...\r\n");
			/* 格式化 */
			res_sd=f_mkfs(path,0,0);							
			
			if(res_sd == FR_OK)
			{
				Uart1_SendStr("》SD卡已成功格式化文件系统。\r\n");
				/* 格式化后，先取消挂载 */
				res_sd = f_mount(NULL,path,1);			
				/* 重新挂载	*/			
				res_sd = f_mount(&fs,path,1);
			}
			else
			{
				LED_D1_0;
				Uart1_SendStr("《《格式化失败。》》\r\n");
				while(1);
			}
		}
		else 
			if(res_sd!=FR_OK)
			{
				printf("！！SD卡挂载文件系统失败。(%d)\r\n",res_sd);
				Uart1_SendStr("！！可能原因：SD卡初始化不成功。\r\n");
				while(1);
			}
			else
			{
				Uart1_SendStr("》文件系统挂载成功，可以进行读写测试\r\n");
			}
}		


/**********************************
写操作初始化函数
功能：文件写入测试，创建新文件；写入数据的标题栏
输入参数：path，(Pointer to the file name)
					filetitle  文件标题内容
返回值：无；
示例："0:TEMP1.CSV"
			"ID.Noumber,Year/Month/Day Hour:Minute:Second,Temp1,Temp2,Temp3,Temp4,Temp5\n"
**********************************/
/*----------------------- 文件系统测试：写测试 -----------------------------*/
void SD_FAT_WriteInitialize(
	const TCHAR* path,/* Pointer to the file name */
	const char* filetitle /*文件标题内容*/
)
{
	/* 打开文件，如果文件不存在则创建它 */
	Uart1_SendStr("\r\n****** 即将进行文件写入测试... ******\r\n");	
	res_sd = f_open(&fnew, path,FA_CREATE_NEW | FA_WRITE ); //这里文件名作为函数参数传入最好
	if ( res_sd == FR_OK )
	{
		Uart1_SendStr("》打开/创建FatFs读写测试文件.csv文件成功，向文件写入数据。\r\n");
    /* 将指定存储区内容写入到文件内 */
		//写入文件标题栏,自动计算字节数
		res_sd=f_write(&fnew,filetitle,strlen(filetitle),&fnum);
    if(res_sd==FR_OK)
    {
      printf("》文件写入成功，写入字节数据：%d\n",fnum);
      printf("》向文件写入的数据为：\r\n%s\r\n",WriteBuffer);
    }
    else
    {
      printf("！！文件写入失败：(%d)\n",res_sd);
    }    
		/* 不再读写，关闭文件 */
    f_close(&fnew);	
	}
	else
	{	
		LED_D1_0;
		Uart1_SendStr("！！打开/创建文件失败。\r\n");
	}
}


/**********************************
文件追加数据函数
功能：数据写入到指定文件，可以实现在已有文件追加数据；
输入参数：path，(Pointer to the file name)
					buff  数据缓冲区地址
返回值：无；
示例："0:TEMP1.CSV"
			WriteBuffer
**********************************/
void SD_FAT_WriteExisiting(
	const TCHAR* path,/* Pointer to the file name */
	const void *buff	/* Pointer to the data to be written */
//	UINT btw
)
{
		res_sd=f_open(&fnew, path,FA_OPEN_EXISTING | FA_WRITE );//打开已经存在的文件，执行写入操作
		res_sd=f_lseek(&fnew,f_size(&fnew)); //搜索指针，实现数据追加
		res_sd=f_write(&fnew,buff,strlen(buff),&fnum);
		f_close(&fnew);			//文件写入结束之后需要关闭文件
}


/**********************************
文件数据读取函数
功能：数据读取到指定缓冲区
输入参数：path，(Pointer to data buffer)
					buff  数据缓冲区地址
返回值：无；
示例："0:TEMP1.CSV"
			ReadBuffer
**********************************/
/*------------------- 文件系统测试：读测试 ------------------------------------*/
void SD_FAT_Read(
	const TCHAR* path/* Pointer to the file name */
//	char* buff	/* Pointer to data buffer */
)
{
	
	Uart1_SendStr("****** 即将进行文件读取测试... ******\r\n");
	res_sd = f_open(&fnew, path, FA_OPEN_EXISTING | FA_READ); 	 
	if(res_sd == FR_OK)
	{
		LED_D2_0;
		Uart1_SendStr("》打开文件成功。\r\n");
		res_sd = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum); 
    if(res_sd==FR_OK)
    {
      printf("》文件读取成功,读到字节数据：%d\r\n",fnum);
      printf("》读取得的文件数据为：\r\n%s \r\n", ReadBuffer);	
    }
    else
    {
      Uart1_SendStr("！！文件读取失败：\n");
    }		
	}
	else
	{
		LED_D1_0;
		Uart1_SendStr("！！打开文件失败。\r\n");
	}
	/* 不再读写，关闭文件 */
	f_close(&fnew);	

}



/**********************************
//SD内文件夹及文件删除函数
//功能：删除文件
//输入参数：字符串——文件路径
//返回值：无
**********************************/
void SD_FAT_DeleteFile(TCHAR* path)
{
	FS_DeleteFolderOrFile(path);
}



/**********************************
取消挂载函数
功能：不再使用文件系统
输入参数：path，(Pointer to data buffer)
返回值：无；
示例："0:"
**********************************/
void SD_FAT_DISMOUNT(const TCHAR* path)
{
		/* 不再使用文件系统，取消挂载文件系统 */
	f_mount(NULL,path,1);
	/* 操作完成，停机 */
}





















//void SD_FAT_STAT()
//{
//		res_sd = f_stat("TEMP1.CSV", filinfo); 
//		if( res_sd )
//		printf("newname.CSV err : %d\r\n", res_sd);
//		else
//		{
//		printf("newname.CSV size : %lu\r\n",filinfo->fsize);
//		printf("fdate : %d\r\n",filinfo->fdate); 
//		printf("ftime : %d\r\n",filinfo->ftime);
//		printf("fattrib : %d\r\n",filinfo->fattrib);
//		}
//}

