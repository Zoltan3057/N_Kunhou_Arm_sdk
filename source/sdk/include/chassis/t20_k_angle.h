#ifndef _CHASSIS_T20_K_ANGLE_2017_08_16_
#define _CHASSIS_T20_K_ANGLE_2017_08_16_

#include <boost/thread.hpp>
#include <map>

#include "MyDefine.h"

class t20_k_angle{

public:
	t20_k_angle();
	~t20_k_angle();

	void load_para();
	void save_para();

	F32 trans_angle( F32 angle );

	bool calibration(F32 &k_angle , const F32 &set_angle , const F32 &current_angle, const F32 &p_step);

	void set_k_para(const F32 &angle,const F32 &k_para);

private:

	std::map<F32,F32> m_k_para_;


	F32 min_angle_k_;
	F32 mid_angle_k_;
	F32 max_angle_k_;

	F32 min_angle_;
	F32 mid_angle_;
	F32 max_angle_;
};


#endif//_CHASSIS_T20_K_ANGLE_2017_08_16_

