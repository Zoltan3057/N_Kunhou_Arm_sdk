/***************************************************************************************************
* Copyright (C), 2015-2016 Suzhou Kunhou Automation Co.,Ltd
* File name:    FileSystemOperation.h
* Description:
* Version:      0.0.1
* History: 
* (1) Author:   taibaidl
*     Date:     2016-06-21
*     Operate:  Create
*     Version:  0.0.1
* Others: 
***************************************************************************************************/
#ifndef _FILE_SYSTEM_OPERATION_H_
#define _FILE_SYSTEM_OPERATION_H_

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <string>
#include <vector>


class FileSystemOperation
{
	//floder
public:
	static bool Exists(const std::string &dir);

	static void CreateFolder(const std::string &dir);

	static void DeleteFileOrFolder(const std::string &dir);

	static void DeleteFolder(const std::string &dir);

	static int FolderIterationForFolder(const std::string        &dir, 
		                                std::vector<std::string> &path_list, 
										std::vector<std::string> &name_list);
	
	static int FolderIteration(const std::string        &dir, 
		                       std::vector<std::string> &file_path_list);

	static int FolderIteration(const std::string        &dir, 
		                       std::vector<std::string> &file_path_list, 
							   std::vector<std::string> &file_name_list);
	
	static int FolderIteration(const std::string        &dir, 
		                       std::vector<std::string> &file_path_list, 
							   std::vector<std::string> &file_name_list, 
		                       std::vector<std::string> &folder_path_list, 
							   std::vector<std::string> &folder_name_list);

	//file
public:
	static bool CopyAFile(const std::string &path_src, 
		                  const std::string &file_name, 
						  const std::string &path_dst);
};
#endif	//_FILE_SYSTEM_OPERATION_H_
