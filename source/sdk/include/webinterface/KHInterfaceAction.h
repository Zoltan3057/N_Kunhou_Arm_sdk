/***************************************************************************************************
* Copyright (C), 2015-2016 Suzhou Kunhou Automation Co.,Ltd
* File name:    KHInterfaceAction.h
* Description:
* Version:      0.0.1
* History: 
* (1) Author:   taibaidl
*     Date:     2016-06-03
*     Operate:  Create
*     Version:  0.0.1
* Others: 
***************************************************************************************************/
#ifndef _KH_INTERFACE_ACTION_H_
#define _KH_INTERFACE_ACTION_H_

#include <map>
#include <vector>
#include <boost/function.hpp>

#include "XmlRpc.h"
#include "KHTypes.h"
#include "KHCommon.h"


/***************************************************************************************************
* INTERFACE   
***************************************************************************************************/
class KHInterfaceAction
{
public:
	KHInterfaceAction();
	~KHInterfaceAction();

public:
	void Init(const char *host);

public:
	std::vector<STRING> GetAllFunction();

	bool CallBackFun(const STRING             &func_name, 
		             std::map<STRING, STRING> &input_data, 
		             STRING                   &out_data);

private:
	void bindFuns();

private:
	F32 calVectorTheta(F32 start_x, 
		               F32 start_y,
		               F32 end_x,
		               F32 end_y);

private:
	//
	bool reboot(std::map<STRING, STRING> &input_data, 
		        STRING                   &out_data);

	/************ page monitor **********************************/
	//
	bool startRobot(std::map<STRING, STRING> &input_data, 
		           STRING                   &out_data);

	//
	bool setRobotPos(std::map<STRING, STRING> &input_data, 
		             STRING                   &out_data);

	bool getRobotPos(std::map<STRING, STRING> &input_data,
					STRING                    &out_data);
	bool getLaser(std::map<STRING, STRING> &input_data,
					STRING                    &out_data);
	bool getGlobalPath(std::map<STRING, STRING> &input_data,
						STRING                    &out_data);

	bool getStatus(std::map<STRING, STRING> &input_data,
						STRING                    &out_data);

	bool setTargetPos(std::map<STRING, STRING> &input_data, 
		              STRING                   &out_data);

	//
	bool selectFixPath(std::map<STRING, STRING> &input_data, 
		               STRING                   &out_data);

	bool recodePath(std::map<STRING, STRING> &input_data,
		            STRING                   &out_data);

	bool savePath(std::map<STRING, STRING> &input_data, 
		          STRING                   &out_data);

	//
	bool runPath(std::map<STRING, STRING> &input_data, 
		         STRING                   &out_data);

	bool stopRobot(std::map<STRING, STRING> &input_data, 
		           STRING                   &out_data);

	//
	bool setPathFileState(std::map<STRING, STRING> &input_data,
		                  STRING                   &out_data);

	bool setRobotLaserFileState(std::map<STRING, STRING> &input_data,
		                        STRING                   &out_data);

	//
	bool getMapList(std::map<STRING, STRING> &input_data, 
		            STRING                   &out_data);

	bool setAmclMap(std::map<STRING, STRING> &input_data,
		            STRING                   &out_data);

	bool setNaviMap(std::map<STRING, STRING> &input_data,
		            STRING                   &out_data);

	/************ page mapping**********************************/
	bool recodeMap(std::map<STRING, STRING> &input_data,
		           STRING                   &out_data);

	bool stopRecodeMap(std::map<STRING, STRING> &input_data,
		               STRING                   &out_data);

	bool robotForwardRecode(std::map<STRING, STRING> &input_data,
		                    STRING                   &out_data);

	bool robotBackwardRecode(std::map<STRING, STRING> &input_data,
			                    STRING                   &out_data);

	bool robotTurnLeftRecode(std::map<STRING, STRING> &input_data,
		                     STRING                   &out_data);

	bool robotTurnRightRecode(std::map<STRING, STRING> &input_data,
		                      STRING                   &out_data);

	bool robotPauseRecode(std::map<STRING, STRING> &input_data,
		                  STRING                   &out_data);

	bool getRecodeMaps(std::map<STRING, STRING> &input_data,
		               STRING                   &out_data);

	bool mapping(std::map<STRING, STRING> &input_data,
		         STRING                   &out_data);

	bool pause(std::map<STRING, STRING> &input_data,
			     STRING                   &out_data);
	bool getMapProcess(std::map<STRING, STRING> &input_data,
							STRING                   &out_data);
private:
	bool setFixPath(const STRING  &user_path);

	bool getFileList(STRING &dir_path, 
		             STRING &out_data,
		             STRING label);

	bool setParam(const STRING  &param_name, 
		          const STRING  &value);

private:
	typedef boost::function< bool (std::map<STRING, STRING> &input_data, 
		                           STRING                   &out_data)> ActionCallback;

	std::map< STRING, KHInterfaceAction::ActionCallback > functions_;

private:
	XmlRpc::XmlRpcClient *action_rpc_client_;
	XmlRpc::XmlRpcClient *param_rpc_client_;

	float  laser_dx_;
};

#endif  //_KH_INTERFACE_ACTION_H_
