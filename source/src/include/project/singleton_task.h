#ifndef _SINGLETON_TASK_H_
#define _SINGLETON_TASK_H_

#include "MyDefine.h"
#include <list>
#include <boost/thread.hpp>
#include <iostream>
#include "TimerDiff.h"
#include "RobotStruct.h"
#include "buffer_con.hpp"

#include "control_data.h"

class singleton_task
{
public:
	singleton_task();
	~singleton_task();

protected:
private:
public:
	U8 io_;
	bool is_updown_task_;
	U8 is_up_;
	std::string path_;
	U8 is_get_;
	U8 is_down_;
	F32 aim_x_;
	F32 aim_y_;

	void set_runtask(std::string &path, U8 &io, U8 &is_get, F32 &aim_x, F32 &aim_y);
	void set_updowntask(U8 &is_up);

	bool do_task(Control_data_read &InStatus);

	//fork updown reference
private:
	bool b_send_plc_;
	void send_up_down();
	bool check_updown_over(Control_data_read &InStatus);
	void publish_function(S32 function_id);

private:
	bool b_send_nav_;
	void send_run_path(std::string &path);
	bool check_arriver(Control_data_read &InStatus);
	bool b_in_moved_;
	bool check_obs(Control_data_read &InStatus);
public:
	void set_robot_stop();

	F32 put_stop_dis_;
	F32 get_stop_dis_;
};


#endif

