/**************************************************************************************************
* 文件名：	base_type_arm.h
* 功能：	ARM编译器环境下的基础数据类型定义
* 作者：	胡志鹏
* 日期：	2023.09.25
* 备注：	
**************************************************************************************************/
#ifndef	BASE_TYPE_ARM_H
	#define	BASE_TYPE_ARM_H
	//---------------------------------------------------------------------------------------------
	typedef unsigned char  BOOL;
	typedef unsigned char  BOOLEAN;
	
	typedef          char  INT8;
//	typedef          int   INT16;
	typedef          short INT16;
	typedef          long  INT32;
	
	typedef   signed char  INT8S;
//	typedef   signed int   INT16S;
	typedef   signed short INT16S;
	typedef   signed long  INT32S;
	
	typedef unsigned char  INT8U;
//	typedef unsigned int   INT16U;
	typedef unsigned short INT16U;
	typedef unsigned long  INT32U;
	
	typedef          char  I8;
//	typedef          int   I16;
	typedef          short I16;
	typedef          long  I32;
	
	typedef   signed char  S8;
//	typedef   signed int   S16;
	typedef   signed short S16;
	typedef   signed long  S32;
	
	typedef unsigned char  U8;
//	typedef unsigned int   U16;
	typedef unsigned short U16;
	typedef unsigned long  U32;
	
	typedef          float FP32;
	//---------------------------------------------------------------------------------------------
#endif	// BASE_TYPE_ARM_H

