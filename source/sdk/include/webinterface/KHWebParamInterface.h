/***************************************************************************************************
* Copyright (C), 2015-2016 Suzhou Kunhou Automation Co.,Ltd
* File name:    KHWebParamInterface.h
* Description:
* Version:      0.0.1
* History: 
* (1) Author:   taibaidl
*     Date:     2016-06-03
*     Operate:  Create
*     Version:  0.0.1
* Others: 
***************************************************************************************************/
#ifndef _KH_WEB_PARAM_INTERFACE_H_
#define _KH_WEB_PARAM_INTERFACE_H_

#include "jsoncpp/json.h"
#include "webinterface/KHInterfaceParam.h"


class KHWebParamInterface
{
public:
	KHWebParamInterface();
	~KHWebParamInterface();

public:
	void Init(const char *host);

public:
	bool Process(const Json::Value  &root,
		         Json::Value        &js_out_node);

private:
	bool setParam(const Json::Value  &param_root);

	bool getParam(const Json::Value  &param_root,
		          Json::Value        &js_out_node);

	bool setMultiParams(const Json::Value  &param_root);

	bool getAllParam(const Json::Value  &param_root,
		             Json::Value        &js_out_node);

private:
	void cvt2JsonValue(const STRING   &data,
		               const STRING   &data_type,
		               Json::Value    &js_val);

private:
	KHInterfaceParam  kh_interface_param_;
}; 

#endif //_KH_WEB_PARAM_INTERFACE_H_
