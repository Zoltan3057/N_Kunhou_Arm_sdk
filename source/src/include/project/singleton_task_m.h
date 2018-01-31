#ifndef _SINGLETON_TASK_M_H_
#define _SINGLETON_TASK_M_H_

#include "MyDefine.h"
#include <list>
#include <boost/thread.hpp>
#include <iostream>
#include "TimerDiff.h"
#include "RobotStruct.h"
#include "buffer_con.hpp"

#include "project/node_topology.h"
#include "project/project.h"

#include "control_data.h"

#include "project/singleton_task.h"

class singleton_manage
{
public:
	singleton_manage();
	~singleton_manage();

protected:
private:
//	std::string pathnameA;
//	std::string pathnameB;
	std::string path_record_;
	std::string path;
	std::string type_name;
	Project project_;
	Manage<Node_topology> node_list;
	void Path_creat(const std::vector<Data_Path_C> &data_, std::string &path_,F32 &aim_x,F32 &aim_y);
	void Add_RunTask(std::string &path, U8 &io, U8 &is_get, F32 &aim_x, F32 &aim_y);
	void Add_UpDownTask(U8 &is_up);
	void Do_Up_Down(U8 &is_up);
	void Do_Runpath(U8 &io, std::string &path, U8 &fork_id_, Control_data_read &InStatus);

	U8 task_id_;
	std::string get_task_name();

	void Path_save(std::vector<SOdomSpeed> data_output, std::string &path);

public:
	std::list<singleton_task> task_list;	//node table
	std::vector<singleton_task> current_task_;



	bool Init_Path_Creat();
	bool task_resolve(std::vector<Data_Server_t> data_s, Control_data_read &InStatus);
	bool do_task_over(Control_data_read &InStatus);
	void cancle_task();
	void test_data(U32 id);

};


#endif






