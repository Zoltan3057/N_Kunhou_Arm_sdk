/***************************************************************************************************
* Copyright (C), 2015-2016 Suzhou Kunhou Automation Co.,Ltd
* File name:    KHConfigData.h
* Description:
* Version:      0.0.1
* History: 
* (1) Author:   taibaidl
*     Date:     2016-07-04
*     Operate:  Create
*     Version:  0.0.1
* Others: 
***************************************************************************************************/
#include <map>
#include "KHTypes.h"
#include "KHBoostInterproc.h"

/***************************************************************************************************
* class CfgBIDataRemove
***************************************************************************************************/
class CfgBIDataRemove
{
public:
	CfgBIDataRemove()
	{
		remove();
	}

	~CfgBIDataRemove()
	{
		remove();
	}

private:
	void remove()
	{
		BI::named_mutex::remove("cfg_data_mtx");
	}
};


/***************************************************************************************************
* class KHConfigData
***************************************************************************************************/
class KHConfigData
{
public:
	KHConfigData();
	~KHConfigData();

public:
	bool Init(std::map<STRING, STRING> &cfgs);

	bool SetParam(const STRING &param_name,
			      const STRING &param_val);

	bool GetParam(const STRING &param_name,
			      STRING       &param_val);

private:
	CfgBIDataRemove   shm_data_remove_;
	BI::named_mutex   cfg_data_mtx_;

private:
	std::map<STRING, STRING> cfgs_;
};
