#ifndef _CHASSIS_T20_K_RPM_2017_08_16_
#define _CHASSIS_T20_K_RPM_2017_08_16_

#include <boost/thread.hpp>
#include <map>

#include "MyDefine.h"

class t20_k_rpm{

public:
	t20_k_rpm();
	~t20_k_rpm();

	void load_para();
	void save_para();

	F32 trans_rpm( F32 rpm );

	bool calibration(F32 &k_rpm , const F32 &set_rpm , const F32 &current_rpm );

	void set_k_para(const F32 &rpm,const F32 &k_para);

private:

	std::map<F32,F32> m_k_para_;
	F32 speed2rpm(const F32 &speed, const F32 &adia);

	F32 min_rpm_k_;
	F32 mid_rpm_k1_;
	F32 mid_rpm_k2_;
	F32 max_rpm_k_;

	F32 min_rpm_;
	F32 mid_rpm1_;
	F32 mid_rpm2_;
	F32 max_rpm_;
};


#endif//_CHASSIS_T20_K_RPM_2017_08_16_

