#ifndef _SIM_PATH_WANGHONGTAO_2017_05_21_
#define _SIM_PATH_WANGHONGTAO_2017_05_21_


#include <vector>
#include <boost/thread.hpp>

#include "MyDefine.h"
#include "TimerDiff.h"
#include "RobotStruct.h"
#include "buffer_con.hpp"



typedef struct _SPath_Point
{
	U32 id_pos_;
	F32 pos_x_;
	F32 pos_y_;

}SPath_Point;

#define POINT_STEP 0.15

class sim_path
{
public:

	sim_path();
	~sim_path();

	void init(const F32 &Vx,const F32 &W,const F32 &max_len,F32 dx);
	void debug_print();

	std::vector<SPath_Point> v_point_;

private:

	S32 point_size_;

};

typedef struct _SPath_VW
{
	U32 id_;
	F32 Vx_;
	F32 Vw_;
	S32 weight_;
	
	sim_path one_path_;

}SPath_VW;

#endif//_SIM_PATH_WANGHONGTAO_2017_05_21_
