#ifndef _ROBOT_STRUCT_WANGHONGTAO_20150831_
#define _ROBOT_STRUCT_WANGHONGTAO_20150831_

#include "KHTypes.h"
#include <vector>
#define LASER_COUNT 541
#define FPGA_LASER_BIN_CNT 512

#define FPGA_LASER_BIN_CNT_256 256
#define FPGA_LASER_BIN_CNT_512 512

#define LASER_DIFF ((LASER_COUNT - FPGA_LASER_BIN_CNT) / 2)


#define LASER_COUNT_URG 1081

typedef enum{
	IDLE = 0,
	PLAN = 1,
	MOVE = 2,
	RECOVERY = 3,
	ERR = 4
}Status;

typedef enum{
	FRONT = 1,
	BACK = 2,
	LEFT = 3,
	RIGHT = 4
}Direction;

typedef enum
{
	dynamic = 1,
	fix = 2
}MODEL_TYPE;

typedef enum{
	GLOBAL_PLANNER_ERR = 0,
	LOCAL_PLANNER_ERR = 1,
	OBSTACLE_DETECT = 2,
	AMCL_ERR = 3,
	OUT_OF_LINE_ERR = 4,
	STOPPING_ERR = 5,
	OBSTACLE_STOP = 6
}ERR_CODE;

typedef enum{
	DIFF2WD = 0,
	FORK_STEER = 1,
	FORK_DIFF = 2,
	SINGLE = 4,
	MULIT = 8,
	OMNI = 16,
	TYPE1 = 32,
	TYPE2 = 64
}Motion_type;

typedef struct _SSerialNum
{

	U32 serial_num;
	U32 motion_type;
	U32 d1;
	U32 d2;
	U8 main_v;
	U8 sub_v;
	U8 c1;
	U8 c2;
}SSerialNum;

typedef struct _SGridXY
{
	U16 x_;
	U16 y_;
	S8 value_;

	S8 dir_;
	S8 speed_;
	S8 ptype_;

}SGridXY;

typedef struct _SLaser
{
	U32 stamp_;
	F32 start_angle_;
	F32 resolution_;	//
	F32 range_max_;
	F32 range_min_;

	F32 data_[LASER_COUNT];
	U8  seg_[LASER_COUNT];
	U32 used_;

} SLaser;

typedef struct _SLaser_para
{
	std::string laser_ip_;
	bool reverse_;
	F32 laser_start_angle_;
	F32 laser_range_max_;
	F32 laser_range_min_;
	F32 laser_dx_;
	F32 laser_dy_;

} SLaser_para;

typedef struct _Shape_xy
{
	F32 x1_;
	F32 y1_;
	F32 x2_;
	F32 y2_;
	F32 x3_;
	F32 y3_;
	F32 x4_;
	F32 y4_;
}Shape_xy;

typedef struct _SLaserXY
{

	F32 x_[LASER_COUNT];
	F32 y_[LASER_COUNT];
	U8 used_[LASER_COUNT];

} SLaserXY;

typedef struct _SPos
{
	F32 x_;
	F32 y_;
	F32 th_;

}SPos;

typedef struct _SSpeed
{
	F32 vx_;	//speed vx_
	F32 vy_;	//speed vy_
	F32 vw_;	//speed vw_

}SSpeed;

typedef struct _SOdomSpeed
{
	F32 x_;
	F32 y_;
	F32 th_;

	F32 vx_;	//speed vx_
	F32 vy_;	//speed vy_
	F32 vw_;	//speed vw_

}SOdomSpeed;

typedef struct _Sxy
{
	F32 x_;
	F32 y_;

}Sxy;

typedef struct _S16xy
{
	S16 x_;
	S16 y_;

}S16xy;

typedef struct _SLaser_xy
{
	Sxy xy_[LASER_COUNT];
	U32 used_size_;

}SLaser_xy;


typedef struct _SPair_Odo_Laser
{

	SPos pos_;
	SLaser laser_;

}SPair_Odo_Laser;

typedef struct _SInit_Pos
{
	SPos pos_;

	U32 psize_;

	F32 para1_;
	F32 para2_;
	F32 para3_;
	F32 para4_;

}SInit_Pos;

typedef struct _STarget_Pos
{
	U8 model_;
	U8  stop_;
	U16 tmp2_;

	F32 x_;
	F32 y_;
	F32 th_;

}STarget_Pos;

typedef struct _SRunPath
{
	U8 replan_;
	U8 backward_;
	U8 tmp1_;
	U8 tmp2_;
	char path_name_[200];

}SRunPath;

typedef struct _SAMCL_RES
{

	F32 entropy_;
	F32 hz_;
	S32 stamp_;

}SAMCL_RES;

typedef struct _SparticleFAW
{

	F32 x;
	F32 y;
	F32 th;
	F32 aw;

}SparticleFAW;

typedef struct _SparticlePub
{
	U32 pwsize_;
	SparticleFAW data_w_[8192];

}SparticlePub;

typedef struct _Srecoder
{

	char cmd_file_[200];

}Srecoder;

typedef struct _SRunStatus
{

	Status status_;
	U8 err_code_;
	U8 arrived_;
	U8 back_;

	F32 sx_;
	F32 sy_;
	F32 sw_;

	F32 cx_;
	F32 cy_;
	F32 cw_;

}SRunStatus;

#define MAX_DI 24
#define MAX_DO 24
#define MAX_AI 8
#define MAX_AO 8

typedef struct _SDIAI
{
	U8 di_[MAX_DI];
	S32 ai_[MAX_AI];

}SDIAI;

typedef struct _SDOAO
{
	U8 do_[MAX_DO];
	S32 ao_[MAX_AO];

}SDOAO;

typedef struct _SDO
{
	U32 used_;
	U8 id_[MAX_DO];
	U8 do_[MAX_DO];

}SDO;

typedef struct _SAO
{
	U32 used_;
	U8 id_[MAX_AO];
	S32 ao_[MAX_AO];

}SAO;

typedef struct _SDI
{
	U32 used_;
	U8 id_[MAX_DI];
	S32 di_[MAX_DI];

}SDI;

typedef struct _SSub_task_upload
{
	U32 agv_id_;

	U32 sub_task_index_;
	U32 prority_;
	int current_step_;
	int max_step_;

	std::vector<SGridXY> run_path_;

}SSub_task_upload;

typedef struct _SSub_task_path
{
	U32 agv_id_;

	U32 sub_task_index_;
	U32 prority_;

	std::vector<SGridXY> run_path_;

}SSub_task_path;

typedef struct _SSub_task_path_aux
{
	U32 agv_id_;

	U32 sub_task_index_;
	U32 prority_;
	U32 max_step_;
}SSub_task_path_aux;


typedef struct _SSub_task_fnc
{
	U32 agv_id_;

	U32 sub_task_index_;
	U32 prority_;
	U32 function_code_; // 0 fork down 1 fork up  3 pos

	int current_step_;
	int max_step_;

}SSub_task_fnc;

typedef struct _SFork_state
{
	U32 fnc_code_;
	S32 para_;               //0 - 10000    mm
	S32 current_status_;     //0:action 1:finished

}SFork_state;


typedef struct _SControl
{
	U32 agv_id_;
	F32 max_speed_;
	int pause_run_slow;

}SControl;

#define THREAD_COUNTS   10

typedef struct _Health_Status
{
	F32 frequency[THREAD_COUNTS] ;     //unit :  Hz
	int error_code[THREAD_COUNTS];     //flag,  0:not work   1:low   2: normal
}Health_Status;


typedef struct _MapProcess
{
	U8 processbar_type_;            //  1: mapping  , 2: set_amclmap, 3: set_globalmap
	S32 counts_;
	S32 total_;
}MapProcess;


#endif//_ROBOT_STRUCT_WANGHONGTAO_20150831_

