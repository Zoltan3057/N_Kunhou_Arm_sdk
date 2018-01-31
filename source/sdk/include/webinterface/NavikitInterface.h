/***************************************************************************************************
* Copyright (C), 2015-2016 Suzhou Kunhou Automation Co.,Ltd
* File name:    NavikitInterface.h
* Description:
* Version:      0.0.1
* History: 
* (1) Author:   taibaidl
*     Date:     2016-09-04
*     Operate:  Create
*     Version:  0.0.1
* Others: 
***************************************************************************************************/
#ifndef _NAVIKIT_INTERFACE_H_
#define _NAVIKIT_INTERFACE_H_


#include <string>

//
typedef void* HANDLE;

//
void NavikitInterfaceCreate(const char  *host,
	                        HANDLE      *handle);

//
int NavikitInterfaceCall(HANDLE      *handle, 
	                     std::string  command, 
	                     std::string&  result);

//
void NavikitInterfaceRelease(HANDLE  *handle);



#endif // _NAVIKIT_INTERFACE_H_
