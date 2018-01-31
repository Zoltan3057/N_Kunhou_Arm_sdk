/***************************************************************************************************
* Copyright (C), 2015-2016 Suzhou Kunhou Automation Co.,Ltd
* File name:    KHConfig.h
* Description:
* Version:      0.0.1
* History: 
* (1) Author:   taibaidl
*     Date:     2016-06-23
*     Operate:  Create
*     Version:  0.0.1
* Others: 
***************************************************************************************************/
#ifndef _KH_CONFIG_H_
#define _KH_CONFIG_H_

#include <map>
#include <boost/thread/mutex.hpp>
#include "KHTypes.h"
#include "KHIniParser.h"
#include "cfgserver/KHConfigData.h"


struct KHParam
{
public:
	KHParam(S32    key, 
		    STRING value, 
			STRING type, 
		    STRING param_name,
			STRING ini_parser_label, 
			bool   is_outer_param)
		: key_(key)
		, value_(value)
		, type_(type)
		, param_name_(param_name)
		, ini_parser_label_(ini_parser_label)
		, is_outer_param_(is_outer_param)
	{
	};

	KHParam(): key_(-1){};

public:
	S32     key_;
	STRING  value_;
	STRING  type_;
	STRING  param_name_;
	STRING  ini_parser_label_;
	bool    is_outer_param_;	
};


/***************************************************************************************************
* INTERFACE   
***************************************************************************************************/
class KHConfig
{
public:
	KHConfig();
	~KHConfig();

public:
	bool SetParam(const S32    &key,
			      const STRING &value);

	bool GetParam(const S32   &key,
			      STRING      &value,
			      STRING      &data_type);

	bool GetAllOuterParam(std::map<S32, KHParam> &outer_params);

	S32 GetKey(STRING param_name);

private:
	bool initParam();

	bool loadCfgData(INIParser &cfg_parser, 
		             STRING     path_label);

private: 
	STRING                      cfg_path_;
	std::map<STRING, INIParser> ini_parser_map_;

private:
	KHConfigData  cfg_shm_data_;

private:
	std::map<STRING, S32> key_map_;

private:
	std::map<S32, KHParam>  outer_params_;
	std::map<S32, KHParam>  inner_params_;

private:
	boost::mutex data_mutex_;
};

#endif  // _KH_CONFIG_H_

