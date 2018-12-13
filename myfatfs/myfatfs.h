#ifndef __MYFATFS_H
#define __MYFATFS_H

#include "ff.h"
#include "usart.h"
#include "led.h"

//函数申明区
void SD_FAT_Initialize(const TCHAR* path);  //文件系统初始化函数,挂载SD卡
void SD_FAT_WriteInitialize(const TCHAR* path,const char* filetitle);//文件写入测试，同时将标题栏写入
void SD_FAT_WriteExisiting(	const TCHAR* path,const void *buff);//文件追加数据函数
void SD_FAT_Read(	const TCHAR* path/*,char* buff*/);//文件数据读取函数
void SD_FAT_DeleteFile(TCHAR* path);//文件删除函数
void SD_FAT_DISMOUNT(const TCHAR* path);//取消挂载函数
//void SD_FAT_STAT();







#endif


