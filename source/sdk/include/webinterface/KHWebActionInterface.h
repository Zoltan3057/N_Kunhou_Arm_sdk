/***************************************************************************************************
* Copyright (C), 2015-2016 Suzhou Kunhou Automation Co.,Ltd
* File name:    KHWebActionInterface.h
* Description:
* Version:      0.0.1
* History: 
* (1) Author:   taibaidl
*     Date:     2016-06-03
*     Operate:  Create
*     Version:  0.0.1
* Others: 
***************************************************************************************************/
#ifndef _KH_WEB_ACTION_INTERFACE_H_
#define _KH_WEB_ACTION_INTERFACE_H_

#include "jsoncpp/json.h"
#include "KHInterfaceAction.h"


class KHWebActionInterface
{
public:
	KHWebActionInterface();
	~KHWebActionInterface();

public:
	void Init(const char *host);

public:
	bool Process(const Json::Value  &action_root,
		         Json::Value        &js_out_node);

private:
	bool getAllFunction(Json::Value &js_out_node);

	bool callBackFunction(const Json::Value  &action_root,
	                      Json::Value        &js_out_node);

private:
	KHInterfaceAction kh_interface_action_;

}; 

#endif // _KH_WEB_ACTION_INTERFACE_H_
