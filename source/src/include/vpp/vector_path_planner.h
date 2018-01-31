#ifndef _VECTOR_PATH_PLANNER_WANGHONGTAO_2017_05_21_
#define _VECTOR_PATH_PLANNER_WANGHONGTAO_2017_05_21_


#include "MyDefine.h"
#include <boost/thread.hpp>
#include <vector>

#include "TimerDiff.h"
#include "RobotStruct.h"
#include "buffer_con.hpp"

#include "vpp/sim_patn.h"

class vector_path_planner
{
public:

	vector_path_planner();
	~vector_path_planner();

	void init(const F32 &Vx,const F32 &maxW,S32 path_count,const F32 &max_len,const F32 &dx);
	std::vector<SPath_VW> get_vw_list();

	void debug_print(const std::string &name);

protected:


private:

	std::vector<SPath_VW> vw_list_;

protected://

};

#endif//_VECTOR_PATH_PLANNER_WANGHONGTAO_2017_05_21_
