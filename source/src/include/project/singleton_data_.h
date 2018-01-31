#ifndef _SINGLETON_DATA_H_
#define _SINGLETON_DATA_H_

#include "MyDefine.h"
#include <list>
#include <vector>
#include <boost/thread.hpp>
#include <iostream>
#include "TimerDiff.h"
#include "RobotStruct.h"
#include "buffer_con.hpp"

#include "control_data.h"

#include "XmlRpc.h"

class singleton_data
{
public:
	singleton_data();
	~singleton_data();

protected:
public:
	std::vector<Data_Server_t> data_server_t;
	typedef enum{NONE_TASK,NEW_TASK,CANCLE_TASK,TASK_RUN,TASK_PAUSE} TASK_STATUS;

private:

	XmlRpc::XmlRpcClient* rpc_cilent_;
public:
	bool init(const std::string str_ip,const int &port,int AGV_id);
	void Data_Receive(TASK_STATUS &task_status);
	int RunTask();

	int agv_id_;
	int port_;

};

#endif
