/***************************************************************************************************
* Copyright (C), 2015-2016 Suzhou Kunhou Automation Co.,Ltd
* File name:    KHCfgServer.h
* Description:
* Version:      0.0.1
* History: 
* (1) Author:   taibaidl
*     Date:     2016-06-22
*     Operate:  Create
*     Version:  0.0.1
* Others: 
***************************************************************************************************/
#ifndef _CFG_SERVE_H_
#define _CFG_SERVE_H_

#include "xmlrpc/XmlRpc.h"
#include "cfgserver/KHConfig.h"

using namespace XmlRpc;

class CfgServer : public XmlRpcServerMethod
{
public:
	CfgServer(XmlRpcServer* server);
	~CfgServer();

public:
	void execute(XmlRpcValue &params, 
		         XmlRpcValue &result);

private:
	void setParam(XmlRpcValue &params, 
		          XmlRpcValue &result);

	void getParam(XmlRpcValue &params, 
		          XmlRpcValue &result);

	void getAllOuterParam( XmlRpcValue &result );

private:
	KHConfig kh_cfg_;
};



#endif//_CFG_SERVE_H_
