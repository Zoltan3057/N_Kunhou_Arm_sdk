/***************************************************************************************************
* Copyright (C), 2015-2016 Suzhou Kunhou Automation Co.,Ltd
* File name:    KHInterfaceParam.h
* Description:
* Version:      0.0.1
* History: 
* (1) Author:   taibaidl
*     Date:     2016-06-01
*     Operate:  Create
*     Version:  0.0.1
* Others: 
***************************************************************************************************/
#ifndef _KH_INTERFACE_PARAM_H_
#define _KH_INTERFACE_PARAM_H_

#include "KHTypes.h"
#include "xmlrpc/XmlRpc.h"


class KHInterfaceParam
{
public:
	KHInterfaceParam();
	~KHInterfaceParam();

public:
	void Init(const char *host);

public:
	bool SetParam(const S32    &key, 
		          const STRING &value);

	bool SetParam(const STRING  &param_name, 
		          const STRING  &value);

	bool GetParam(const S32     &key,
		          STRING        &value,
		          STRING       &data_type);

	bool GetParam(const STRING  &param_name,
		          STRING        &value);

	bool GetAllOuterParam(XmlRpc::XmlRpcValue &result);

private:
	XmlRpc::XmlRpcClient *rpc_client_;
};

#endif //_KH_INTERFACE_PARAM_H_

