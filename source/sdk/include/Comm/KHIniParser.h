/***************************************************************************************************
* Copyright (C), 2015-2016 Suzhou Kunhou Automation Co.,Ltd
* File name:    KHIniParser.h
* Description:
* Version:      0.0.1
* History: 
* (1) Author:   taibaidl
*     Date:     2016-04-20
*     Operate:  Create
*     Version:  0.0.1
* Others: 
***************************************************************************************************/
#ifndef _INI_PARSER_H_
#define _INI_PARSER_H_

#include <string>
#include <map>
#include <vector>


struct SubNode
{
public:
	void InsertElement(std::string key, 
                       std::string value)
	{
		sub_node_.insert(std::pair<std::string, std::string>(key, value));		
	}

	std::map<std::string, std::string> GetAllSubNodes()
	{
		return sub_node_;
	}; 

public:
	std::map<std::string, std::string> sub_node_;
};


class INIParser
{
public:
	INIParser();
	~INIParser();

public:
	//
	bool ReadINI(std::string path);
	bool WriteINI(std::string path);
	bool WriteINI();

	//
	void SetValue(std::string root, 
                  std::string key, 
                  std::string value);
	
    std::string GetValue(std::string root, 
                         std::string key);

	SubNode GetSubNode(const std::string &root);

	std::map<std::string, SubNode> GetINIMap();

public:
	inline int GetSize()
	{
		return map_ini_.size();
	}

	inline void Clear()
	{
		map_ini_.clear();
	}

private:
	std::string &trimString(std::string &str);

private:
	std::vector<std::string>       roots_;
	std::map<std::string, SubNode> map_ini_;
	std::string file_path_;
};

#endif // _INI_PARSER_H_
