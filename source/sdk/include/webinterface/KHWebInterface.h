/***************************************************************************************************
* Copyright (C), 2015-2016 Suzhou Kunhou Automation Co.,Ltd
* File name:    KHWebInterface.h
* Description:
* Version:      0.0.1
* History: 
* (1) Author:   taibaidl
*     Date:     2016-06-02
*     Operate:  Create
*     Version:  0.0.1
* Others: 
***************************************************************************************************/
#ifndef _KH_WEB_INTERFACE_H_
#define _KH_WEB_INTERFACE_H_

#include "jsoncpp/json.h"
#include "webinterface/KHWebParamInterface.h"
#include "webinterface/KHWebActionInterface.h"

class KHWebInterface
{
public:
	KHWebInterface(const char *host = "127.0.0.1");
	~KHWebInterface();

public:
	void Process(const STRING  &input,
                 STRING        &output);

private:
	KHWebParamInterface   param_interface_;
	KHWebActionInterface  action_interface_;
}; 

#endif // _KH_WEB_INTERFACE_H_
