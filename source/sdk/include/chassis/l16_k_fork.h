#ifndef _CHASSIS_L16_K_FORK_2017_09_20_
#define _CHASSIS_L16_K_FORK_2017_09_20_

#include <boost/thread.hpp>
#include <map>

#include "MyDefine.h"

class l16_k_fork{

public:
	l16_k_fork();
	~l16_k_fork();

	void load_para();
	void save_para();

	S32 trans_fork( S32 &fork );

	bool calibration(F32 &k_fork , S32 &dio_fork , S32 &current_fork);

	void set_k_para(const S32 &fork,const F32 &k_para);

private:

	std::map<S32,F32> m_k_para_;


	F32 min_fork_k_;
	F32 max_fork_k_;

	S32 min_fork_;
	S32 max_fork_;
};


#endif//_CHASSIS_T20_K_ANGLE_2017_08_16_

