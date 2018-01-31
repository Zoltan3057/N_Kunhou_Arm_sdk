#ifndef _CONTROL_DATA_WANGHONGTAO_2016_11_19_
#define _CONTROL_DATA_WANGHONGTAO_2016_11_19_


#include "MyDefine.h"
// #include <boost/thread.hpp>
// 
// #include "TimerDiff.h"
// #include "buffer_con.hpp"

typedef struct _SControl_data_read{
	U16 angle;
	U16 sequence;
	
	U8 id;
	U8 stop;
//	U8 fork_up;
//	U8 fork_down;
	U8 emergency;
	U8 hight_speed;

	U8 mid_speed;
	U8 low_speed;
	U8 hlow_speed;
	U8 manual;

	U8 low_power;
	U8 action_over;
	U8 new_current_station;
	U8 new_target_station;

	U8 forklift_cargo;
	U8 fork_left_;
	U8 fork_right_;
	U8 forklift_status;


	U16 get_current_station;
	U16 get_target_station;

	F32  amcl_pos_x_;
	F32  amcl_pos_y_;
	F32  amcl_pos_th_;

}Control_data_read;

typedef struct _SControl_data_write{
	

	U16 sequence;
	U16 function;

	U8 run_status;
	U8 stop_status;
	U8 fork_up;
	U8 fork_down;
	U8 set_fork_up;
	U8 set_fork_down;
	U8 obstacle;
	U8 err_code;

	U8 show_hight_speed;
	U8 show_mid_speed;
	U8 show_low_speed;
	U8 show_hlow_speed;
	U8 cls_current_station;
	U8 cls_target_station;
	U8 do_function;
	U8 back1;



// 	U16 show_current_station;
// 	U16 show_target_station;

}Control_data_write;


typedef struct _SData_Server_t
{
	U32  id_;
	F32  speed_;
	U8  function_;
	U8  io_;
	U8  straight_;
	U8  back1_;
	U16 back2_;

}Data_Server_t;

typedef struct _SData_Path_C
{
	U32  id_;
	F32  speed_;
	U8  function_;
	U8  io_;
	F32 x_;
	F32 y_;

}Data_Path_C;



#endif//_CONTROL_DATA_WANGHONGTAO_2016_11_19_



